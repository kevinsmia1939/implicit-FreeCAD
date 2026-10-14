// SPDX-License-Identifier: LGPL-2.1-or-later
#include "implicit/CylinderIntersection.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace implicit {
namespace {
NumericalSettings chartSettings(const ImplicitSurface& field, const CylinderPatch& patch)
{
    auto settings = field.settings();
    // A unit angular displacement has world-space speed radius. A chart-space
    // stopping tolerance must therefore account for the map's maximum stretch,
    // rather than treating radians and model lengths as interchangeable.
    settings.distanceTolerance /= std::max(1.0, patch.radius);
    return settings;
}
class CylinderChart final : public ImplicitSurface {
public:
    CylinderChart(const ImplicitSurface& field, const CylinderPatch& patch)
        : ImplicitSurface({{0, patch.heightMinimum, -1},
                           {2 * std::acos(-1.0), patch.heightMaximum, 1}}, chartSettings(field, patch)),
          field(field), patch(patch), axis(normalized(patch.axis)),
          u(normalized(patch.radialDirection)), v(cross(axis, u))
    {
        if (std::abs(dot(axis, u)) > 1e-10) {
            throw std::invalid_argument("cylinder axis and radial direction must be perpendicular");
        }
    }
    Vec3 map(double angle, double height) const
    {
        return patch.origin + axis * height
            + (u * std::cos(angle) + v * std::sin(angle)) * patch.radius;
    }
    double evaluate(const Vec3& point) const override
    {
        return field.evaluate(map(point.x, point.y));
    }
    void validateBounds() const
    {
        // Analytic coordinate extrema cover every angle, not just sampled nodes.
        const Vec3 a = patch.origin + axis * patch.heightMinimum;
        const Vec3 b = patch.origin + axis * patch.heightMaximum;
        const Vec3 extent {patch.radius * std::hypot(u.x, v.x),
                           patch.radius * std::hypot(u.y, v.y),
                           patch.radius * std::hypot(u.z, v.z)};
        const Vec3 minimum {std::min(a.x, b.x) - extent.x,
                            std::min(a.y, b.y) - extent.y,
                            std::min(a.z, b.z) - extent.z};
        const Vec3 maximum {std::max(a.x, b.x) + extent.x,
                            std::max(a.y, b.y) + extent.y,
                            std::max(a.z, b.z) + extent.z};
        if (!field.domain().contains(minimum, field.settings().distanceTolerance)
            || !field.domain().contains(maximum, field.settings().distanceTolerance)) {
            throw std::invalid_argument("cylinder patch must lie inside the implicit domain");
        }
    }
private:
    const ImplicitSurface& field;
    CylinderPatch patch;
    Vec3 axis, u, v;
};
bool finite(const Vec3& point)
{
    return std::isfinite(point.x) && std::isfinite(point.y) && std::isfinite(point.z);
}
}

std::vector<IntersectionCurve> CylinderIntersection::trace(const ImplicitSurface& surface,
                                                          const CylinderPatch& patch)
{
    if (!finite(patch.origin) || !finite(patch.axis) || !finite(patch.radialDirection)
        || norm(patch.axis) == 0 || norm(patch.radialDirection) == 0
        || !std::isfinite(norm(patch.axis)) || !std::isfinite(norm(patch.radialDirection))
        || !std::isfinite(patch.radius) || patch.radius <= 0
        || !std::isfinite(patch.heightMinimum) || !std::isfinite(patch.heightMaximum)
        || patch.heightMinimum >= patch.heightMaximum || patch.angleCells < 3
        || patch.heightCells == 0 || patch.angleCells > 4096 || patch.heightCells > 4096) {
        throw std::invalid_argument("invalid finite cylinder patch or sampling budget");
    }
    const CylinderChart chart(surface, patch);
    chart.validateBounds();
    PlanePatch plane;
    plane.uMinimum = 0;
    plane.uMaximum = 2 * std::acos(-1.0);
    plane.vMinimum = patch.heightMinimum;
    plane.vMaximum = patch.heightMaximum;
    plane.uCells = patch.angleCells;
    plane.vCells = patch.heightCells;
    auto curves = PlaneIntersection::trace(chart, plane);
    for (auto& curve : curves) {
        for (auto& point : curve.points) point = chart.map(point.x, point.y);
    }
    const double tolerance = surface.settings().distanceTolerance * 16;
    auto coincides = [tolerance](const Vec3& a, const Vec3& b) {
        return norm(a - b) <= tolerance;
    };
    // Join only endpoints. Internal crossings/singularities are not resolved.
    for (std::size_t i = 0; i < curves.size(); ++i) {
        auto& curve = curves[i];
        if (curve.points.empty()) continue;
        bool changed = true;
        while (!curve.closed && changed) {
            changed = false;
            if (coincides(curve.points.front(), curve.points.back())) {
                curve.points.back() = curve.points.front();
                curve.closed = true;
                break;
            }
            for (std::size_t j = i + 1; j < curves.size(); ++j) {
                auto& other = curves[j];
                if (other.closed || other.points.empty()) continue;
                if (coincides(curve.points.front(), other.points.front())
                    || coincides(curve.points.front(), other.points.back())) {
                    std::reverse(curve.points.begin(), curve.points.end());
                }
                if (coincides(curve.points.back(), other.points.back())) {
                    std::reverse(other.points.begin(), other.points.end());
                }
                if (!coincides(curve.points.back(), other.points.front())) continue;
                curve.points.insert(curve.points.end(), other.points.begin() + 1, other.points.end());
                curve.maximumResidual = std::max(curve.maximumResidual, other.maximumResidual);
                other.points.clear();
                changed = true;
                break;
            }
        }
    }
    curves.erase(std::remove_if(curves.begin(), curves.end(), [](const auto& curve) {
        return curve.points.empty();
    }), curves.end());
    return curves;
}
}
