#include "implicit/MarchingTetrahedra.h"

#include <array>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>

namespace implicit {
namespace {

constexpr std::array<std::array<int, 4>, 6> tetrahedra {{
    {{0, 5, 1, 6}}, {{0, 1, 2, 6}}, {{0, 2, 3, 6}},
    {{0, 3, 7, 6}}, {{0, 7, 4, 6}}, {{0, 4, 5, 6}},
}};

Vec3 interpolate(const Vec3& a, const Vec3& b, double fa, double fb)
{
    const double denominator = fa - fb;
    const double amount = std::abs(denominator) < 1.0e-15 ? 0.5 : fa / denominator;
    return a + (b - a) * amount;
}

void addTriangle(const ImplicitSurface& surface,
                 TriangleMesh& mesh,
                 Vec3 a,
                 Vec3 b,
                 Vec3 c)
{
    Vec3 na = surface.surfaceNormal(a);
    Vec3 nb = surface.surfaceNormal(b);
    Vec3 nc = surface.surfaceNormal(c);
    const Vec3 faceNormal = cross(b - a, c - a);
    if (dot(faceNormal, na + nb + nc) < 0.0) {
        std::swap(b, c);
        std::swap(nb, nc);
    }
    const std::size_t first = mesh.vertices.size();
    mesh.vertices.insert(mesh.vertices.end(), {a, b, c});
    mesh.normals.insert(mesh.normals.end(), {na, nb, nc});
    mesh.triangles.push_back({first, first + 1, first + 2});
}

void polygoniseTetrahedron(const ImplicitSurface& surface,
                           const std::array<Vec3, 4>& points,
                           const std::array<double, 4>& values,
                           TriangleMesh& mesh)
{
    std::array<int, 4> inside {};
    std::array<int, 4> outside {};
    std::size_t insideCount = 0;
    std::size_t outsideCount = 0;
    for (int index = 0; index < 4; ++index) {
        if (values[index] <= 0.0) inside[insideCount++] = index;
        else outside[outsideCount++] = index;
    }
    if (insideCount == 0 || insideCount == 4) return;

    if (insideCount == 1 || insideCount == 3) {
        const bool invert = insideCount == 3;
        const int singleton = invert ? outside[0] : inside[0];
        const auto& others = invert ? inside : outside;
        Vec3 vertices[3];
        for (int edge = 0; edge < 3; ++edge) {
            vertices[edge] = interpolate(points[singleton], points[others[edge]],
                                         values[singleton], values[others[edge]]);
        }
        if (invert) addTriangle(surface, mesh, vertices[0], vertices[2], vertices[1]);
        else addTriangle(surface, mesh, vertices[0], vertices[1], vertices[2]);
        return;
    }

    const Vec3 a = interpolate(points[inside[0]], points[outside[0]],
                               values[inside[0]], values[outside[0]]);
    const Vec3 b = interpolate(points[inside[0]], points[outside[1]],
                               values[inside[0]], values[outside[1]]);
    const Vec3 c = interpolate(points[inside[1]], points[outside[0]],
                               values[inside[1]], values[outside[0]]);
    const Vec3 d = interpolate(points[inside[1]], points[outside[1]],
                               values[inside[1]], values[outside[1]]);
    addTriangle(surface, mesh, a, b, c);
    addTriangle(surface, mesh, b, d, c);
}

} // namespace

TriangleMesh MarchingTetrahedra::extract(const ImplicitSurface& surface,
                                         GridResolution resolution)
{
    if (resolution.x == 0 || resolution.y == 0 || resolution.z == 0) {
        throw std::invalid_argument("all extraction grid dimensions must be positive");
    }
    const Aabb& bounds = surface.domain();
    const Vec3 step {(bounds.maximum.x - bounds.minimum.x) / resolution.x,
                     (bounds.maximum.y - bounds.minimum.y) / resolution.y,
                     (bounds.maximum.z - bounds.minimum.z) / resolution.z};
    const std::size_t nx = resolution.x + 1;
    const std::size_t ny = resolution.y + 1;
    const std::size_t nz = resolution.z + 1;
    std::vector<double> samples(nx * ny * nz);
    auto offset = [nx, ny](std::size_t x, std::size_t y, std::size_t z) {
        return z * nx * ny + y * nx + x;
    };
    auto point = [&](std::size_t x, std::size_t y, std::size_t z) {
        return bounds.minimum + Vec3 {step.x * x, step.y * y, step.z * z};
    };

    for (std::size_t z = 0; z < nz; ++z) {
        for (std::size_t y = 0; y < ny; ++y) {
            for (std::size_t x = 0; x < nx; ++x) {
                samples[offset(x, y, z)] = surface.evaluate(point(x, y, z));
            }
        }
    }

    TriangleMesh mesh;
    mesh.vertices.reserve(resolution.x * resolution.y * resolution.z * 9);
    mesh.normals.reserve(mesh.vertices.capacity());
    mesh.triangles.reserve(resolution.x * resolution.y * resolution.z * 3);
    constexpr std::array<std::array<int, 3>, 8> corners {{
        {{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}, {{0, 1, 0}},
        {{0, 0, 1}}, {{1, 0, 1}}, {{1, 1, 1}}, {{0, 1, 1}},
    }};

    for (std::size_t z = 0; z < resolution.z; ++z) {
        for (std::size_t y = 0; y < resolution.y; ++y) {
            for (std::size_t x = 0; x < resolution.x; ++x) {
                std::array<Vec3, 8> cubePoints;
                std::array<double, 8> cubeValues;
                for (std::size_t corner = 0; corner < corners.size(); ++corner) {
                    const std::size_t cx = x + corners[corner][0];
                    const std::size_t cy = y + corners[corner][1];
                    const std::size_t cz = z + corners[corner][2];
                    cubePoints[corner] = point(cx, cy, cz);
                    cubeValues[corner] = samples[offset(cx, cy, cz)];
                }
                for (const auto& tetrahedron : tetrahedra) {
                    std::array<Vec3, 4> tetraPoints;
                    std::array<double, 4> tetraValues;
                    for (std::size_t vertex = 0; vertex < 4; ++vertex) {
                        tetraPoints[vertex] = cubePoints[tetrahedron[vertex]];
                        tetraValues[vertex] = cubeValues[tetrahedron[vertex]];
                    }
                    polygoniseTetrahedron(surface, tetraPoints, tetraValues, mesh);
                }
            }
        }
    }
    return mesh;
}

} // namespace implicit
