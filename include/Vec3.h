#pragma once
#include <cmath>

struct Vec3 {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};

    constexpr Vec3() = default;
    constexpr Vec3(float x_val, float y_val, float z_val) : x(x_val), y(y_val), z(z_val) {}

    [[nodiscard]] Vec3 operator+(const Vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    [[nodiscard]] Vec3 operator-(const Vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    [[nodiscard]] Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }

    [[nodiscard]] float dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    [[nodiscard]] float length() const { return std::sqrt(dot(*this)); }

    [[nodiscard]] Vec3 normalize() const {
        const float l = length();
        return l > 0.0f ? *this * (1.0f / l) : Vec3(0.0f, 0.0f, 0.0f);
    }
};
