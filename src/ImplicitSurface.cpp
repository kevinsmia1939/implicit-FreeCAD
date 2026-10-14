#include "implicit/ImplicitSurface.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace implicit {
namespace {

bool finite(const Vec3& value)
{
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

std::array<double, 4> solveLinear4(std::array<std::array<double, 5>, 4> matrix)
{
    for (std::size_t column = 0; column < 4; ++column) {
        std::size_t pivot = column;
        for (std::size_t row = column + 1; row < 4; ++row) {
            if (std::abs(matrix[row][column]) > std::abs(matrix[pivot][column])) {
                pivot = row;
            }
        }
        if (std::abs(matrix[pivot][column]) < 1.0e-15) {
            throw std::runtime_error("singular projection system");
        }
        std::swap(matrix[column], matrix[pivot]);
        const double divisor = matrix[column][column];
        for (std::size_t entry = column; entry < 5; ++entry) {
            matrix[column][entry] /= divisor;
        }
        for (std::size_t row = 0; row < 4; ++row) {
            if (row == column) {
                continue;
            }
            const double scale = matrix[row][column];
            for (std::size_t entry = column; entry < 5; ++entry) {
                matrix[row][entry] -= scale * matrix[column][entry];
            }
        }
    }
    return {matrix[0][4], matrix[1][4], matrix[2][4], matrix[3][4]};
}

} // namespace

ImplicitSurface::ImplicitSurface(Aabb domain, NumericalSettings settings)
    : domain_(domain)
    , settings_(settings)
{
    setDomain(domain);
    setSettings(settings);
}

void ImplicitSurface::setDomain(const Aabb& domain)
{
    if (!domain.isValid() || !finite(domain.minimum) || !finite(domain.maximum)) {
        throw std::invalid_argument("implicit surface domain must be finite and non-empty");
    }
    domain_ = domain;
}

void ImplicitSurface::setSettings(const NumericalSettings& settings)
{
    if (!(settings.functionTolerance > 0.0) || !(settings.distanceTolerance > 0.0)
        || !(settings.gradientStep > 0.0) || settings.maxIterations == 0
        || settings.raySamples == 0 || !std::isfinite(settings.functionTolerance)
        || !std::isfinite(settings.distanceTolerance) || !std::isfinite(settings.gradientStep)) {
        throw std::invalid_argument("invalid numerical settings");
    }
    settings_ = settings;
}

bool ImplicitSurface::analyticalGradient(const Vec3&, Vec3&) const
{
    return false;
}

Vec3 ImplicitSurface::numericalGradient(const Vec3& point) const
{
    const double h = settings_.gradientStep;
    const Vec3 dx {h, 0.0, 0.0};
    const Vec3 dy {0.0, h, 0.0};
    const Vec3 dz {0.0, 0.0, h};
    return {(evaluate(point + dx) - evaluate(point - dx)) / (2.0 * h),
            (evaluate(point + dy) - evaluate(point - dy)) / (2.0 * h),
            (evaluate(point + dz) - evaluate(point - dz)) / (2.0 * h)};
}

Vec3 ImplicitSurface::gradient(const Vec3& point) const
{
    Vec3 result;
    if (analyticalGradient(point, result)) {
        return result;
    }
    return numericalGradient(point);
}

Vec3 ImplicitSurface::surfaceNormal(const Vec3& point) const
{
    const Vec3 result = gradient(point);
    if (norm(result) <= settings_.functionTolerance) {
        throw std::runtime_error("surface normal is undefined at a critical point");
    }
    return normalized(result);
}

PointClassification ImplicitSurface::classify(const Vec3& point, double tolerance) const
{
    if (tolerance < 0.0) {
        tolerance = settings_.functionTolerance;
    }
    if (!std::isfinite(tolerance) || tolerance < 0.0) {
        throw std::invalid_argument("classification tolerance must be finite and non-negative");
    }
    const double value = evaluate(point);
    if (value < -tolerance) return PointClassification::Negative;
    if (value > tolerance) return PointClassification::Positive;
    return PointClassification::OnSurface;
}

ProjectionResult ImplicitSurface::project(const Vec3& query) const
{
    ProjectionResult result;
    result.point = query;
    double lambda = 0.0;

    // Solve the stationarity equations x-q+lambda*grad(F)=0, F=0.  The
    // gradient's Jacobian (the Hessian of F) is evaluated numerically so
    // subclasses only need F and optionally its first analytical derivative.
    for (std::size_t iteration = 0; iteration < settings_.maxIterations; ++iteration) {
        result.iterations = iteration + 1;
        const Vec3 g = gradient(result.point);
        const double f = evaluate(result.point);
        const Vec3 stationarity = result.point - query + lambda * g;
        result.residual = f;

        if (std::abs(f) <= settings_.functionTolerance
            && norm(stationarity) <= settings_.distanceTolerance) {
            result.converged = true;
            break;
        }
        if (squaredNorm(g) <= settings_.functionTolerance * settings_.functionTolerance) {
            break;
        }

        const double h = settings_.gradientStep;
        std::array<std::array<double, 3>, 3> hessian {};
        for (std::size_t axis = 0; axis < 3; ++axis) {
            Vec3 offset;
            if (axis == 0) offset.x = h;
            if (axis == 1) offset.y = h;
            if (axis == 2) offset.z = h;
            const Vec3 difference = (gradient(result.point + offset)
                                   - gradient(result.point - offset)) / (2.0 * h);
            hessian[0][axis] = difference.x;
            hessian[1][axis] = difference.y;
            hessian[2][axis] = difference.z;
        }

        std::array<std::array<double, 5>, 4> system {};
        const std::array<double, 3> gv {g.x, g.y, g.z};
        const std::array<double, 3> sv {stationarity.x, stationarity.y, stationarity.z};
        for (std::size_t row = 0; row < 3; ++row) {
            for (std::size_t column = 0; column < 3; ++column) {
                system[row][column] = (row == column ? 1.0 : 0.0)
                                        + lambda * hessian[row][column];
            }
            system[row][3] = gv[row];
            system[row][4] = -sv[row];
            system[3][row] = gv[row];
        }
        system[3][4] = -f;

        try {
            const auto step = solveLinear4(system);
            Vec3 spatialStep {step[0], step[1], step[2]};
            const double maxStep = std::max({domain_.maximum.x - domain_.minimum.x,
                                             domain_.maximum.y - domain_.minimum.y,
                                             domain_.maximum.z - domain_.minimum.z});
            const double stepLength = norm(spatialStep);
            if (stepLength > maxStep) {
                spatialStep = spatialStep * (maxStep / stepLength);
            }
            result.point += spatialStep;
            lambda += step[3];
        }
        catch (const std::runtime_error&) {
            // A normal-direction Newton step remains useful near a singular
            // KKT matrix and supplies a deterministic failure path.
            result.point = result.point - g * (f / squaredNorm(g));
        }
    }

    result.residual = evaluate(result.point);
    result.distance = norm(result.point - query);
    if (!result.converged) {
        const Vec3 stationarity = result.point - query + lambda * gradient(result.point);
        result.converged = std::abs(result.residual) <= settings_.functionTolerance
                        && norm(stationarity) <= 10.0 * settings_.distanceTolerance;
    }
    return result;
}

double ImplicitSurface::refineRoot(const Vec3& origin,
                                   const Vec3& direction,
                                   double lower,
                                   double upper) const
{
    double fLower = evaluate(origin + direction * lower);
    double fUpper = evaluate(origin + direction * upper);
    if (std::abs(fLower) <= settings_.functionTolerance) return lower;
    if (std::abs(fUpper) <= settings_.functionTolerance) return upper;

    for (std::size_t iteration = 0; iteration < settings_.maxIterations; ++iteration) {
        double candidate = 0.5 * (lower + upper);
        if (fUpper != fLower) {
            const double secant = upper - fUpper * (upper - lower) / (fUpper - fLower);
            if (secant > lower && secant < upper) {
                candidate = secant;
            }
        }
        const double fCandidate = evaluate(origin + direction * candidate);
        if (std::abs(fCandidate) <= settings_.functionTolerance
            || std::abs(upper - lower) * norm(direction) <= settings_.distanceTolerance) {
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
    return 0.5 * (lower + upper);
}

std::vector<RayHit> ImplicitSurface::intersectRay(const Vec3& origin,
                                                  const Vec3& direction,
                                                  double minimumParameter,
                                                  double maximumParameter) const
{
    if (!finite(origin) || !finite(direction) || squaredNorm(direction) == 0.0
        || !(minimumParameter < maximumParameter)) {
        throw std::invalid_argument("ray requires a direction and an ordered finite interval");
    }
    if (!std::isfinite(minimumParameter) || !std::isfinite(maximumParameter)) {
        throw std::invalid_argument("ray interval must be finite");
    }

    std::vector<RayHit> hits;
    const double increment = (maximumParameter - minimumParameter)
                           / static_cast<double>(settings_.raySamples);
    double previousT = minimumParameter;
    double previousF = evaluate(origin + direction * previousT);

    auto appendHit = [&](double parameter) {
        if (!hits.empty()
            && std::abs(parameter - hits.back().parameter) * norm(direction)
                <= 10.0 * settings_.distanceTolerance) {
            return;
        }
        const Vec3 point = origin + direction * parameter;
        hits.push_back({parameter, point, surfaceNormal(point), std::abs(evaluate(point))});
    };

    if (std::abs(previousF) <= settings_.functionTolerance) appendHit(previousT);
    for (std::size_t sample = 1; sample <= settings_.raySamples; ++sample) {
        const double currentT = minimumParameter + static_cast<double>(sample) * increment;
        const double currentF = evaluate(origin + direction * currentT);
        if (std::abs(currentF) <= settings_.functionTolerance) {
            appendHit(currentT);
        }
        else if (std::signbit(currentF) != std::signbit(previousF)
                 && std::abs(previousF) > settings_.functionTolerance) {
            appendHit(refineRoot(origin, direction, previousT, currentT));
        }
        previousT = currentT;
        previousF = currentF;
    }
    return hits;
}

} // namespace implicit

