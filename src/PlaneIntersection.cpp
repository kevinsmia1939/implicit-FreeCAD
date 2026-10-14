#include "implicit/PlaneIntersection.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace implicit {
namespace {

struct EdgeKey {
    bool vertical;
    std::size_t i;
    std::size_t j;

    bool operator==(const EdgeKey& other) const
    {
        return vertical == other.vertical && i == other.i && j == other.j;
    }
};

struct EdgeKeyHash {
    std::size_t operator()(const EdgeKey& key) const
    {
        std::size_t seed = key.i + 0x9e3779b97f4a7c15ULL;
        seed ^= key.j + 0x9e3779b97f4a7c15ULL + (seed << 6U) + (seed >> 2U);
        seed ^= static_cast<std::size_t>(key.vertical) + 0x9e3779b97f4a7c15ULL
              + (seed << 6U) + (seed >> 2U);
        return seed;
    }
};

struct PreparedPatch {
    Vec3 origin;
    Vec3 uDirection;
    Vec3 vDirection;
    double uMinimum;
    double vMinimum;
    double uStep;
    double vStep;
    std::size_t uCells;
    std::size_t vCells;

    Vec3 point(std::size_t i, std::size_t j) const
    {
        return origin + uDirection * (uMinimum + static_cast<double>(i) * uStep)
             + vDirection * (vMinimum + static_cast<double>(j) * vStep);
    }
};

PreparedPatch prepare(const ImplicitSurface& surface, const PlanePatch& patch)
{
    if (patch.uCells == 0 || patch.vCells == 0
        || !(patch.uMinimum < patch.uMaximum) || !(patch.vMinimum < patch.vMaximum)) {
        throw std::invalid_argument("plane patch requires positive cells and ordered bounds");
    }
    const double uLength = norm(patch.uDirection);
    const double vLength = norm(patch.vDirection);
    if (!(uLength > 0.0) || !(vLength > 0.0) || !std::isfinite(uLength)
        || !std::isfinite(vLength)) {
        throw std::invalid_argument("plane patch directions must be finite and non-zero");
    }
    const Vec3 uDirection = patch.uDirection / uLength;
    const Vec3 vDirection = patch.vDirection / vLength;
    if (std::abs(dot(uDirection, vDirection)) > 1.0e-10) {
        throw std::invalid_argument("plane patch directions must be perpendicular");
    }

    PreparedPatch result {patch.origin,
                          uDirection,
                          vDirection,
                          patch.uMinimum,
                          patch.vMinimum,
                          (patch.uMaximum - patch.uMinimum) / patch.uCells,
                          (patch.vMaximum - patch.vMinimum) / patch.vCells,
                          patch.uCells,
                          patch.vCells};
    for (const auto& corner : std::array<std::pair<std::size_t, std::size_t>, 4> {{
             {0, 0}, {patch.uCells, 0}, {patch.uCells, patch.vCells}, {0, patch.vCells}}}) {
        if (!surface.domain().contains(result.point(corner.first, corner.second),
                                       surface.settings().distanceTolerance)) {
            throw std::invalid_argument("plane patch must lie inside the implicit domain");
        }
    }
    return result;
}

Vec3 refineEdgeRoot(const ImplicitSurface& surface, const Vec3& a, const Vec3& b,
                    double fa, double fb)
{
    const auto& settings = surface.settings();
    if (std::abs(fa) <= settings.functionTolerance) return a;
    if (std::abs(fb) <= settings.functionTolerance) return b;
    Vec3 lower = a;
    Vec3 upper = b;
    double fLower = fa;
    double fUpper = fb;

    for (std::size_t iteration = 0; iteration < settings.maxIterations; ++iteration) {
        double amount = 0.5;
        if (fUpper != fLower) {
            const double secant = -fLower / (fUpper - fLower);
            if (secant > 0.05 && secant < 0.95) amount = secant;
        }
        const Vec3 candidate = lower + (upper - lower) * amount;
        const double fCandidate = surface.evaluate(candidate);
        if (std::abs(fCandidate) <= settings.functionTolerance
            || norm(upper - lower) <= settings.distanceTolerance) {
            return candidate;
        }
        if (std::signbit(fCandidate) == std::signbit(fLower)) {
            lower = candidate;
            fLower = fCandidate;
        }
        else {
            upper = candidate;
            fUpper = fCandidate;
        }
    }
    return (lower + upper) * 0.5;
}

} // namespace

std::vector<IntersectionCurve> PlaneIntersection::trace(const ImplicitSurface& surface,
                                                        const PlanePatch& patch)
{
    const PreparedPatch plane = prepare(surface, patch);
    const std::size_t width = plane.uCells + 1;
    const std::size_t height = plane.vCells + 1;
    auto sampleIndex = [width](std::size_t i, std::size_t j) { return j * width + i; };
    std::vector<double> samples(width * height);
    for (std::size_t j = 0; j < height; ++j) {
        for (std::size_t i = 0; i < width; ++i) {
            samples[sampleIndex(i, j)] = surface.evaluate(plane.point(i, j));
        }
    }

    std::vector<Vec3> vertices;
    std::vector<std::pair<std::size_t, std::size_t>> segments;
    std::unordered_map<EdgeKey, std::size_t, EdgeKeyHash> edgeVertices;
    auto edgeVertex = [&](const EdgeKey& edge) {
        const auto existing = edgeVertices.find(edge);
        if (existing != edgeVertices.end()) return existing->second;
        const std::size_t i2 = edge.i + (edge.vertical ? 0 : 1);
        const std::size_t j2 = edge.j + (edge.vertical ? 1 : 0);
        const Vec3 a = plane.point(edge.i, edge.j);
        const Vec3 b = plane.point(i2, j2);
        const Vec3 root = refineEdgeRoot(surface, a, b,
                                         samples[sampleIndex(edge.i, edge.j)],
                                         samples[sampleIndex(i2, j2)]);
        const std::size_t index = vertices.size();
        vertices.push_back(root);
        edgeVertices.emplace(edge, index);
        return index;
    };

    for (std::size_t j = 0; j < plane.vCells; ++j) {
        for (std::size_t i = 0; i < plane.uCells; ++i) {
            const std::array<double, 4> values {
                samples[sampleIndex(i, j)], samples[sampleIndex(i + 1, j)],
                samples[sampleIndex(i + 1, j + 1)], samples[sampleIndex(i, j + 1)]};
            const std::array<EdgeKey, 4> edges {{
                {false, i, j}, {true, i + 1, j},
                {false, i, j + 1}, {true, i, j},
            }};
            std::array<int, 4> crossings {};
            std::size_t crossingCount = 0;
            for (int edge = 0; edge < 4; ++edge) {
                const int next = (edge + 1) % 4;
                if (std::signbit(values[edge]) != std::signbit(values[next])) {
                    crossings[crossingCount++] = edge;
                }
            }
            auto addSegment = [&](int firstEdge, int secondEdge) {
                segments.emplace_back(edgeVertex(edges[firstEdge]), edgeVertex(edges[secondEdge]));
            };
            if (crossingCount == 2) {
                addSegment(crossings[0], crossings[1]);
            }
            else if (crossingCount == 4) {
                const Vec3 center = plane.point(i, j)
                                  + plane.uDirection * (0.5 * plane.uStep)
                                  + plane.vDirection * (0.5 * plane.vStep);
                const bool centerMatchesCorner = std::signbit(surface.evaluate(center))
                                               == std::signbit(values[0]);
                if (centerMatchesCorner) {
                    addSegment(3, 0);
                    addSegment(1, 2);
                }
                else {
                    addSegment(0, 1);
                    addSegment(2, 3);
                }
            }
        }
    }

    std::vector<std::vector<std::size_t>> adjacency(vertices.size());
    for (std::size_t segment = 0; segment < segments.size(); ++segment) {
        adjacency[segments[segment].first].push_back(segment);
        adjacency[segments[segment].second].push_back(segment);
    }
    std::vector<bool> used(segments.size(), false);
    std::vector<IntersectionCurve> curves;
    auto walk = [&](std::size_t startVertex, std::size_t startSegment) {
        IntersectionCurve curve;
        std::size_t vertex = startVertex;
        std::size_t segment = startSegment;
        curve.points.push_back(vertices[vertex]);
        while (!used[segment]) {
            used[segment] = true;
            const auto& endpoints = segments[segment];
            vertex = endpoints.first == vertex ? endpoints.second : endpoints.first;
            curve.points.push_back(vertices[vertex]);
            if (vertex == startVertex) {
                curve.closed = true;
                break;
            }
            const auto next = std::find_if(adjacency[vertex].begin(), adjacency[vertex].end(),
                                           [&](std::size_t candidate) { return !used[candidate]; });
            if (next == adjacency[vertex].end()) break;
            segment = *next;
        }
        for (const Vec3& point : curve.points) {
            curve.maximumResidual = std::max(curve.maximumResidual,
                                              std::abs(surface.evaluate(point)));
        }
        if (curve.points.size() >= 2) curves.push_back(std::move(curve));
    };

    // Trace boundary-to-boundary curves first, then the remaining closed loops.
    for (std::size_t vertex = 0; vertex < adjacency.size(); ++vertex) {
        if (adjacency[vertex].size() != 1 || used[adjacency[vertex][0]]) continue;
        walk(vertex, adjacency[vertex][0]);
    }
    for (std::size_t segment = 0; segment < segments.size(); ++segment) {
        if (!used[segment]) walk(segments[segment].first, segment);
    }
    return curves;
}

} // namespace implicit
