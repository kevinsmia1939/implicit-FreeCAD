#pragma once

#include "implicit/ImplicitSurface.h"

#include <array>
#include <cstddef>
#include <vector>

namespace implicit {

struct TriangleMesh {
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<std::array<std::size_t, 3>> triangles;
};

struct GridResolution {
    std::size_t x = 24;
    std::size_t y = 24;
    std::size_t z = 24;
};

class MarchingTetrahedra {
public:
    static TriangleMesh extract(const ImplicitSurface& surface, GridResolution resolution);
};

} // namespace implicit

