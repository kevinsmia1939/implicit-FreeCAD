#include "implicit/GyroidSurface.h"
#include "implicit/MarchingTetrahedra.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

double milliseconds(Clock::time_point begin, Clock::time_point end)
{
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

} // namespace

int main(int argc, char** argv)
{
    std::size_t samplesPerCell = 8;
    if (argc == 2) {
        samplesPerCell = std::max<std::size_t>(2, std::stoul(argv[1]));
    }
    constexpr double period = 10.0;
    const std::vector<std::size_t> cellCounts {1, 3, 5, 10};
    std::cout << "cells,resolution,evaluations,evaluation_ms,mesh_ms,vertices,triangles,mesh_bytes\n";

    volatile double checksum = 0.0;
    for (std::size_t cells : cellCounts) {
        const double length = period * static_cast<double>(cells);
        const implicit::Aabb bounds {{0, 0, 0}, {length, length, length}};
        implicit::GyroidSurface surface(period, 0.0, bounds);
        const std::size_t resolution = cells * samplesPerCell;
        const std::size_t evaluationCount = (resolution + 1) * (resolution + 1) * (resolution + 1);

        const auto evaluationBegin = Clock::now();
        for (std::size_t z = 0; z <= resolution; ++z) {
            for (std::size_t y = 0; y <= resolution; ++y) {
                for (std::size_t x = 0; x <= resolution; ++x) {
                    const double scale = length / static_cast<double>(resolution);
                    checksum = checksum + surface.evaluate({scale * x, scale * y, scale * z});
                }
            }
        }
        const auto evaluationEnd = Clock::now();
        const auto meshBegin = Clock::now();
        const auto mesh = implicit::MarchingTetrahedra::extract(
            surface, {resolution, resolution, resolution});
        const auto meshEnd = Clock::now();
        const std::size_t bytes = mesh.vertices.size() * sizeof(implicit::Vec3)
                                + mesh.normals.size() * sizeof(implicit::Vec3)
                                + mesh.triangles.size() * sizeof(mesh.triangles.front());
        std::cout << cells << ',' << resolution << ',' << evaluationCount << ','
                  << std::fixed << std::setprecision(3)
                  << milliseconds(evaluationBegin, evaluationEnd) << ','
                  << milliseconds(meshBegin, meshEnd) << ','
                  << mesh.vertices.size() << ',' << mesh.triangles.size() << ',' << bytes << '\n';
    }
    if (checksum == 123456789.0) std::cerr << "unreachable: " << checksum << '\n';
}

