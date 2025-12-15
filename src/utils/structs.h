#pragma once
#include <cmath>

struct RGB {
    float r;
    float g;
    float b;
};

struct Vector3 {
    float x, y, z;

    bool equals(const Vector3& other, float epsilon = 0.01f) const {
        return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }
};

struct Vector2 {
    float x, y;

    bool equals(const Vector2& other, float epsilon = 0.01f) const {
        return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon;
    }

    bool operator==(const Vector2& other) const { return equals(other, 10.0f); }
};
