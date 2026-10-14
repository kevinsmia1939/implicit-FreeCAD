#pragma once

#include <cmath>
#include <stdexcept>

namespace implicit {

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vec3 operator+(const Vec3& rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
    Vec3 operator-(const Vec3& rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
    Vec3 operator*(double scale) const { return {x * scale, y * scale, z * scale}; }
    Vec3 operator/(double scale) const
    {
        if (scale == 0.0) {
            throw std::domain_error("division by zero");
        }
        return {x / scale, y / scale, z / scale};
    }
    Vec3& operator+=(const Vec3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
};

inline Vec3 operator*(double scale, const Vec3& value) { return value * scale; }
inline double dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline Vec3 cross(const Vec3& a, const Vec3& b)
{
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}
inline double squaredNorm(const Vec3& value) { return dot(value, value); }
inline double norm(const Vec3& value) { return std::sqrt(squaredNorm(value)); }
inline Vec3 normalized(const Vec3& value)
{
    const double length = norm(value);
    if (length == 0.0) {
        throw std::domain_error("cannot normalize the zero vector");
    }
    return value / length;
}

struct Aabb {
    Vec3 minimum;
    Vec3 maximum;

    bool isValid() const
    {
        return minimum.x < maximum.x && minimum.y < maximum.y && minimum.z < maximum.z;
    }

    bool contains(const Vec3& point, double tolerance = 0.0) const
    {
        return point.x >= minimum.x - tolerance && point.x <= maximum.x + tolerance
            && point.y >= minimum.y - tolerance && point.y <= maximum.y + tolerance
            && point.z >= minimum.z - tolerance && point.z <= maximum.z + tolerance;
    }
};

} // namespace implicit

