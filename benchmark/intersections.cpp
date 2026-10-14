// SPDX-License-Identifier: LGPL-2.1-or-later
#include "implicit/CylinderIntersection.h"
#include "implicit/GyroidSurface.h"
#include "implicit/PlaneIntersection.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

namespace {
using Clock = std::chrono::steady_clock;
double elapsed(Clock::time_point start)
{
    return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}
void curves(const implicit::ImplicitSurface& surface, std::size_t cells,
            const char* operation, std::size_t u, std::size_t v,
            double milliseconds, const std::vector<implicit::IntersectionCurve>& result)
{
    std::size_t points = 0;
    double residual = 0;
    for (const auto& curve : result) {
        points += curve.points.size();
        for (const auto& point : curve.points)
            residual = std::max(residual, std::abs(surface.evaluate(point)));
    }
    std::cout << cells << ',' << operation << ',' << u << ',' << v << ',' << milliseconds
              << ',' << result.size() << ',' << points << ',' << residual << ",\n";
}
}

int main()
{
    try {
        std::cout << std::setprecision(9)
                  << "cells,operation,grid_u,grid_v,elapsed_ms,components,points,max_abs_residual,projection_converged\n";
        for (std::size_t cells : {1, 3, 5, 10}) {
            const double length = 10.0 * cells;
            implicit::NumericalSettings settings;
            settings.raySamples = 128 * cells;
            implicit::GyroidSurface field(10, 0.17, {{0, 0, 0}, {length, length, length}}, settings);
            auto start = Clock::now();
            const auto projection = field.project({length * 0.17, length * 0.29, length * 0.43});
            std::cout << cells << ",projection,0,0," << elapsed(start) << ",1,1,"
                      << std::abs(projection.residual) << ',' << projection.converged << '\n';

            start = Clock::now();
            const auto hits = field.intersectRay({0, length * 0.137, length * 0.321},
                                                  {1, 0, 0}, 0, length);
            const double rayTime = elapsed(start);
            double rayResidual = 0;
            for (const auto& hit : hits) rayResidual = std::max(rayResidual, hit.residual);
            std::cout << cells << ",ray," << settings.raySamples << ",0," << rayTime << ','
                      << hits.size() << ',' << hits.size() << ',' << rayResidual << ",\n";

            implicit::PlanePatch plane;
            plane.origin = {0, 0, length * 0.37};
            plane.uMinimum = plane.vMinimum = 0;
            plane.uMaximum = plane.vMaximum = length;
            plane.uCells = plane.vCells = cells * 24;
            start = Clock::now();
            const auto planeCurves = implicit::PlaneIntersection::trace(field, plane);
            curves(field, cells, "plane", plane.uCells, plane.vCells, elapsed(start), planeCurves);

            implicit::CylinderPatch cylinder;
            cylinder.origin = {length / 2, length / 2, length / 2};
            cylinder.radius = length / 4;
            cylinder.heightMinimum = -length / 2;
            cylinder.heightMaximum = length / 2;
            cylinder.angleCells = cells * 48;
            cylinder.heightCells = cells * 16;
            start = Clock::now();
            const auto cylinderCurves = implicit::CylinderIntersection::trace(field, cylinder);
            curves(field, cells, "cylinder", cylinder.angleCells, cylinder.heightCells,
                   elapsed(start), cylinderCurves);
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
