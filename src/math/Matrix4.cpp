#include "Matrix4.h"
#include <cmath>

namespace Revolt {

Matrix4::Matrix4() {
    LoadIdentity();
}

Matrix4 Matrix4::Identity() {
    Matrix4 result;
    result.LoadIdentity();
    return result;
}

Matrix4 Matrix4::Perspective(float fov, float aspect, float nearPlane, float farPlane) {
    Matrix4 result;
    float f = 1.0f / tan(fov * 0.5f);
    float range = nearPlane - farPlane;
    
    result.m[0] = f / aspect;
    result.m[5] = f;
    result.m[10] = (farPlane + nearPlane) / range;
    result.m[11] = -1.0f;
    result.m[14] = 2.0f * farPlane * nearPlane / range;
    
    return result;
}

void Matrix4::LoadIdentity() {
    for (int i = 0; i < 16; ++i) {
        m[i] = 0.0f;
    }
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void Matrix4::Translate(float x, float y, float z) {
    m[12] += x;
    m[13] += y;
    m[14] += z;
}

void Matrix4::Rotate(float angle, float x, float y, float z) {
    float radians = angle * (3.14159265359f / 180.0f); // Конвертация в радианы
    float c = cos(radians);
    float s = sin(radians);
    
    // Временная реализация только для вращения вокруг Y
    if (y == 1.0f) {
        LoadIdentity();
        m[0] = c;  m[2] = s;
        m[8] = -s; m[10] = c;
        m[5] = 1.0f;
        m[15] = 1.0f;
    }
}

void Matrix4::Multiply(const Matrix4& other) {
    // TODO: Реализовать умножение матриц
}

} // namespace Revolt