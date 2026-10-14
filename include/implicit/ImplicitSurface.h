#pragma once

#include "implicit/Vec3.h"

#include <cstddef>
#include <vector>

namespace implicit {

struct NumericalSettings {
    double functionTolerance = 1.0e-10;
    double distanceTolerance = 1.0e-9;
    double gradientStep = 1.0e-6;
    std::size_t maxIterations = 80;
    std::size_t raySamples = 512;
};

struct ProjectionResult {
    Vec3 point;
    double residual = 0.0;
    double distance = 0.0;
    std::size_t iterations = 0;
    bool converged = false;
};

struct RayHit {
    double parameter = 0.0;
    Vec3 point;
    Vec3 normal;
    double residual = 0.0;
};

enum class PointClassification {
    Negative,
    OnSurface,
    Positive,
};

class ImplicitSurface {
public:
    explicit ImplicitSurface(Aabb domain, NumericalSettings settings = {});
    virtual ~ImplicitSurface() = default;

    virtual double evaluate(const Vec3& point) const = 0;
    Vec3 gradient(const Vec3& point) const;
    Vec3 surfaceNormal(const Vec3& point) const;
    PointClassification classify(const Vec3& point, double tolerance = -1.0) const;
    ProjectionResult project(const Vec3& point) const;
    std::vector<RayHit> intersectRay(const Vec3& origin,
                                     const Vec3& direction,
                                     double minimumParameter,
                                     double maximumParameter) const;

    const Aabb& domain() const { return domain_; }
    const NumericalSettings& settings() const { return settings_; }
    void setDomain(const Aabb& domain);
    void setSettings(const NumericalSettings& settings);

protected:
    virtual bool analyticalGradient(const Vec3& point, Vec3& result) const;

private:
    Vec3 numericalGradient(const Vec3& point) const;
    double refineRoot(const Vec3& origin,
                      const Vec3& direction,
                      double lower,
                      double upper) const;

    Aabb domain_;
    NumericalSettings settings_;
};

} // namespace implicit

