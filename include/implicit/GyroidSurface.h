#pragma once

#include "implicit/ImplicitSurface.h"

namespace implicit {

class GyroidSurface final : public ImplicitSurface {
public:
    GyroidSurface(double period,
                  double level,
                  Aabb domain,
                  NumericalSettings settings = {});

    double evaluate(const Vec3& point) const override;
    double period() const { return period_; }
    double level() const { return level_; }
    void setPeriod(double period);
    void setLevel(double level) { level_ = level; }

protected:
    bool analyticalGradient(const Vec3& point, Vec3& result) const override;

private:
    double period_;
    double level_;
    double waveNumber_;
};

} // namespace implicit

