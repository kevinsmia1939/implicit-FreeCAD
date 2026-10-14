#include "implicit/GyroidSurface.h"

#include <cmath>
#include <stdexcept>

namespace implicit {
namespace {
constexpr double pi = 3.141592653589793238462643383279502884;
}

GyroidSurface::GyroidSurface(double period,
                             double level,
                             Aabb domain,
                             NumericalSettings settings)
    : ImplicitSurface(domain, settings)
    , period_(period)
    , level_(level)
    , waveNumber_(0.0)
{
    setPeriod(period);
}

void GyroidSurface::setPeriod(double period)
{
    if (!(period > 0.0) || !std::isfinite(period)) {
        throw std::invalid_argument("gyroid period must be finite and positive");
    }
    period_ = period;
    waveNumber_ = 2.0 * pi / period_;
}

double GyroidSurface::evaluate(const Vec3& point) const
{
    const double x = waveNumber_ * point.x;
    const double y = waveNumber_ * point.y;
    const double z = waveNumber_ * point.z;
    return std::sin(x) * std::cos(y) + std::sin(y) * std::cos(z)
         + std::sin(z) * std::cos(x) - level_;
}

bool GyroidSurface::analyticalGradient(const Vec3& point, Vec3& result) const
{
    const double x = waveNumber_ * point.x;
    const double y = waveNumber_ * point.y;
    const double z = waveNumber_ * point.z;
    result = {
        waveNumber_ * (std::cos(x) * std::cos(y) - std::sin(z) * std::sin(x)),
        waveNumber_ * (std::cos(y) * std::cos(z) - std::sin(x) * std::sin(y)),
        waveNumber_ * (std::cos(z) * std::cos(x) - std::sin(y) * std::sin(z)),
    };
    return true;
}

} // namespace implicit
