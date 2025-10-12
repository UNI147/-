#include "Matrix4.h"
#include <cmath>

namespace Revolt {

Matrix4::Matrix4() {
    LoadIdentity();
}

Matrix4::Matrix4(const Matrix4& other) {
    for (int i = 0; i < 16; ++i) {
        m[i] = other.m[i];
    }
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
    // УБИРАЕМ преобразование в радианы - углы теперь приходят в градусах
    float radians = angle * (3.14159265359f / 180.0f);
    float c = cos(radians);
    float s = sin(radians);
    float one_minus_c = 1.0f - c;
    
    // Нормализуем ось вращения
    float length = sqrt(x*x + y*y + z*z);
    if (length > 0) {
        x /= length;
        y /= length;
        z /= length;
    }
    
    // Создаем матрицу вращения
    Matrix4 rotation;
    rotation.LoadIdentity();
    
    rotation.m[0] = x*x*one_minus_c + c;
    rotation.m[1] = y*x*one_minus_c + z*s;
    rotation.m[2] = z*x*one_minus_c - y*s;
    
    rotation.m[4] = x*y*one_minus_c - z*s;
    rotation.m[5] = y*y*one_minus_c + c;
    rotation.m[6] = z*y*one_minus_c + x*s;
    
    rotation.m[8] = x*z*one_minus_c + y*s;
    rotation.m[9] = y*z*one_minus_c - x*s;
    rotation.m[10] = z*z*one_minus_c + c;
    
    // Умножаем текущую матрицу на матрицу вращения
    *this = rotation.Multiply(*this);
}

void Matrix4::Scale(float x, float y, float z) {
    // Реализация масштабирования
    m[0] *= x;
    m[5] *= y;
    m[10] *= z;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    return Multiply(other);
}

Matrix4 Matrix4::Multiply(const Matrix4& other) const {
    Matrix4 result;
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i * 4 + j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[i * 4 + j] += m[i * 4 + k] * other.m[k * 4 + j];
            }
        }
    }
    
    return result;
}

} // namespace Revolt