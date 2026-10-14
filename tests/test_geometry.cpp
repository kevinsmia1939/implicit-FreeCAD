#include "implicit/GyroidSurface.h"
#include "implicit/MarchingTetrahedra.h"
#include "implicit/PlaneIntersection.h"
#include "implicit/CylinderIntersection.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

using implicit::Aabb;
using implicit::GyroidSurface;
using implicit::ImplicitSurface;
using implicit::Vec3;

int failures = 0;

void check(bool condition, const std::string& message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void near(double actual, double expected, double tolerance, const std::string& message)
{
    check(std::abs(actual - expected) <= tolerance,
          message + " (actual=" + std::to_string(actual)
              + ", expected=" + std::to_string(expected) + ")");
}

class Sphere final : public ImplicitSurface {
public:
    Sphere() : ImplicitSurface({{-2, -2, -2}, {2, 2, 2}}) {}
    double evaluate(const Vec3& p) const override { return p.x * p.x + p.y * p.y + p.z * p.z - 1.0; }
};

void testGyroidEvaluationAndGradient()
{
    const double pi = std::acos(-1.0);
    GyroidSurface surface(2.0 * pi, 0.0, {{-pi, -pi, -pi}, {pi, pi, pi}});
    near(surface.evaluate({0, 0, 0}), 0.0, 1.0e-14, "origin lies on zero-level gyroid");
    const Vec3 gradient = surface.gradient({0, 0, 0});
    near(gradient.x, 1.0, 1.0e-14, "analytical gradient x");
    near(gradient.y, 1.0, 1.0e-14, "analytical gradient y");
    near(gradient.z, 1.0, 1.0e-14, "analytical gradient z");
    const Vec3 normal = surface.surfaceNormal({0, 0, 0});
    near(implicit::norm(normal), 1.0, 1.0e-14, "normal is unit length");
    near(normal.x, 1.0 / std::sqrt(3.0), 1.0e-14, "normal direction");
}

void testPointClassification()
{
    Sphere sphere;
    check(sphere.classify({0.0, 0.0, 0.0}) == implicit::PointClassification::Negative,
          "sphere origin has negative implicit sign");
    check(sphere.classify({1.0, 0.0, 0.0}) == implicit::PointClassification::OnSurface,
          "sphere boundary is classified on-surface");
    check(sphere.classify({2.0, 0.0, 0.0}) == implicit::PointClassification::Positive,
          "point beyond sphere has positive implicit sign");
    check(sphere.classify({1.0 + 1.0e-5, 0.0, 0.0}, 1.0e-3)
              == implicit::PointClassification::OnSurface,
          "classification honors caller tolerance");
}

void testNumericalGradientFallback()
{
    Sphere sphere;
    const Vec3 gradient = sphere.gradient({1.0, 0.25, -0.5});
    near(gradient.x, 2.0, 1.0e-8, "numerical gradient x");
    near(gradient.y, 0.5, 1.0e-8, "numerical gradient y");
    near(gradient.z, -1.0, 1.0e-8, "numerical gradient z");
}

void testProjection()
{
    Sphere sphere;
    const auto result = sphere.project({2.0, 0.0, 0.0});
    check(result.converged, "sphere projection converges");
    near(result.point.x, 1.0, 1.0e-8, "sphere closest point x");
    near(result.distance, 1.0, 1.0e-8, "sphere projection distance");
    near(result.residual, 0.0, 1.0e-10, "sphere projection residual");

    const double pi = std::acos(-1.0);
    GyroidSurface gyroid(2.0 * pi, 0.0, {{-pi, -pi, -pi}, {pi, pi, pi}});
    const auto gyroidResult = gyroid.project({0.1, 0.1, 0.1});
    check(gyroidResult.converged, "gyroid projection converges");
    near(gyroidResult.residual, 0.0, 1.0e-10, "gyroid projection residual");
    near(gyroidResult.point.x, gyroidResult.point.y, 1.0e-9,
         "symmetric gyroid projection preserves x/y symmetry");
}

void testRayIntersection()
{
    Sphere sphere;
    const auto hits = sphere.intersectRay({-2.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 0.0, 4.0);
    check(hits.size() == 2, "ray finds two sphere intersections");
    if (hits.size() == 2) {
        near(hits[0].parameter, 1.0, 1.0e-8, "first ray parameter");
        near(hits[1].parameter, 3.0, 1.0e-8, "second ray parameter");
        near(hits[0].residual, 0.0, 1.0e-10, "ray root residual");
    }

    const double pi = std::acos(-1.0);
    GyroidSurface gyroid(2.0 * pi, 0.0, {{-pi, -pi, -pi}, {pi, pi, pi}});
    const auto gyroidHits = gyroid.intersectRay({-0.25, 0.0, 0.0}, {1.0, 0.0, 0.0},
                                                 0.0, 0.4);
    check(gyroidHits.size() == 1, "ray finds gyroid zero crossing");
    if (gyroidHits.size() == 1) {
        near(gyroidHits[0].parameter, 0.25, 1.0e-8, "gyroid ray parameter");
    }
}

void testExtraction()
{
    const double pi = std::acos(-1.0);
    GyroidSurface surface(2.0 * pi, 0.0, {{-pi, -pi, -pi}, {pi, pi, pi}});
    const auto mesh = implicit::MarchingTetrahedra::extract(surface, {8, 8, 8});
    check(!mesh.triangles.empty(), "extractor produces gyroid triangles");
    check(mesh.vertices.size() == mesh.normals.size(), "each mesh vertex has a normal");
    check(mesh.vertices.size() == mesh.triangles.size() * 3, "triangle soup indexing is consistent");
    for (const Vec3& vertex : mesh.vertices) {
        check(surface.domain().contains(vertex, 1.0e-12), "mesh vertex remains in domain");
        check(std::abs(surface.evaluate(vertex)) < 0.25,
              "coarse extracted vertex has a bounded implicit residual");
    }
}

void testPlaneIntersection()
{
    const double pi = std::acos(-1.0);
    GyroidSurface surface(2.0 * pi, 0.17, {{-pi, -pi, -pi}, {pi, pi, pi}});
    implicit::PlanePatch patch;
    patch.origin = {0.0, 0.0, 0.0};
    patch.uDirection = {1.0, 0.0, 0.0};
    patch.vDirection = {0.0, 1.0, 0.0};
    patch.uMinimum = -pi;
    patch.uMaximum = pi;
    patch.vMinimum = -pi;
    patch.vMaximum = pi;
    patch.uCells = 48;
    patch.vCells = 48;
    const auto curves = implicit::PlaneIntersection::trace(surface, patch);
    check(!curves.empty(), "plane intersection produces gyroid curves");
    std::size_t pointCount = 0;
    for (const auto& curve : curves) {
        check(curve.points.size() >= 2, "intersection curve contains a segment");
        check(curve.maximumResidual <= surface.settings().functionTolerance * 2.0,
              "intersection curve roots satisfy implicit tolerance");
        pointCount += curve.points.size();
        for (const Vec3& point : curve.points) {
            near(point.z, 0.0, 1.0e-14, "intersection point remains on plane");
            check(surface.domain().contains(point, 1.0e-12),
                  "intersection point remains in implicit domain");
        }
    }
    check(pointCount > 20, "plane intersection has a non-trivial sampled curve");
}

void testValidation()
{
    bool threw = false;
    try {
        GyroidSurface invalid(0.0, 0.0, {{0, 0, 0}, {1, 1, 1}});
    }
    catch (const std::invalid_argument&) {
        threw = true;
    }
    check(threw, "invalid gyroid period is rejected");
}

void testCylinderIntersection()
{
    Sphere sphere;
    implicit::CylinderPatch patch;
    patch.radius = 0.65;
    patch.heightMinimum = -1.5;
    patch.heightMaximum = 1.5;
    patch.angleCells = 40;
    patch.heightCells = 50;
    auto curves = implicit::CylinderIntersection::trace(sphere, patch);
    check(curves.size() == 2, "sphere-cylinder intersection has two circles");
    for (const auto& curve : curves) {
        check(curve.closed, "cylinder seam is joined into a closed loop");
        check(curve.maximumResidual < 2e-9, "cylinder contour field residual");
        for (const auto& point : curve.points) {
            near(std::hypot(point.x, point.y), patch.radius, 1e-12,
                "refined contour remains exactly on cylinder");
            near(std::abs(point.z), std::sqrt(1 - patch.radius * patch.radius), 2e-9,
                "sphere-cylinder circle height");
        }
    }
    patch.axis = {1, 0, 0};
    patch.radialDirection = {0, 1, 0};
    curves = implicit::CylinderIntersection::trace(sphere, patch);
    check(curves.size() == 2, "rotated cylinder has two circles");
    for (const auto& curve : curves) {
        check(curve.closed, "rotated seam is joined");
        for (const auto& point : curve.points) {
            near(std::hypot(point.y, point.z), patch.radius, 1e-12, "rotated radial distance");
        }
    }
    bool rejected = false;
    patch.radius = 4;
    try { (void)implicit::CylinderIntersection::trace(sphere, patch); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected, "out-of-domain cylinder rejected");
}

} // namespace

int main()
{
    testGyroidEvaluationAndGradient();
    testPointClassification();
    testNumericalGradientFallback();
    testProjection();
    testRayIntersection();
    testExtraction();
    testPlaneIntersection();
    testCylinderIntersection();
    testValidation();
    if (failures != 0) {
        std::cerr << failures << " test assertion(s) failed\n";
        return EXIT_FAILURE;
    }
    std::cout << "All implicit geometry tests passed\n";
    return EXIT_SUCCESS;
}

