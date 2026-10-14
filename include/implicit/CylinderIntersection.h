// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include "implicit/PlaneIntersection.h"

namespace implicit {
struct CylinderPatch {
    Vec3 origin;
    Vec3 axis {0, 0, 1};
    Vec3 radialDirection {1, 0, 0};
    double radius = 1;
    double heightMinimum = -1;
    double heightMaximum = 1;
    std::size_t angleCells = 96;
    std::size_t heightCells = 64;
};

class CylinderIntersection {
public:
    // Full 2*pi circumference, finite height. Roots are refined in the cylinder
    // chart, then joined across its periodic seam. The full cylinder must fit
    // inside the field domain. Tangencies/coincident regions are not guaranteed.
    static std::vector<IntersectionCurve> trace(const ImplicitSurface& surface,
                                               const CylinderPatch& patch);
};
}
