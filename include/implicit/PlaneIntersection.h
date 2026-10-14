#pragma once

#include "implicit/ImplicitSurface.h"

#include <cstddef>
#include <vector>

namespace implicit {

struct PlanePatch {
    Vec3 origin;
    Vec3 uDirection {1.0, 0.0, 0.0};
    Vec3 vDirection {0.0, 1.0, 0.0};
    double uMinimum = -1.0;
    double uMaximum = 1.0;
    double vMinimum = -1.0;
    double vMaximum = 1.0;
    std::size_t uCells = 64;
    std::size_t vCells = 64;
};

struct IntersectionCurve {
    std::vector<Vec3> points;
    bool closed = false;
    double maximumResidual = 0.0;
};

class PlaneIntersection {
public:
    // Numerically contours F(origin + u*uDirection + v*vDirection)=0 on a
    // finite rectangular plane patch. Directions are normalized internally
    // and must be mutually perpendicular.
    static std::vector<IntersectionCurve> trace(const ImplicitSurface& surface,
                                                const PlanePatch& patch);
};

} // namespace implicit

