// src/Types.h
#pragma once
#include <cmath>

constexpr float PI = 3.14159265f;
constexpr float TO_RAD = PI / 180.0f;

struct Vec3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;

    Vec3() = default;
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }

    Vec3& operator+=(const Vec3& o) {
        x += o.x; y += o.y; z += o.z;
        return *this;
    }

    float length() const { return std::sqrt(x * x + y * y + z * z); }

    Vec3 normalized() const {
        float len = length();
        if (len < 1e-6f) return Vec3(0, 0, 0); // avoid divide-by-zero
        return Vec3(x / len, y / len, z / len);
    }
};