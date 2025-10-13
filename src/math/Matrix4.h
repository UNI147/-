#pragma once

namespace Revolt {

class Matrix4 {
public:
    float m[16];

    Matrix4();
    Matrix4(const Matrix4& other);
    
    static Matrix4 Identity();
    static Matrix4 Perspective(float fov, float aspect, float nearPlane, float farPlane);
    
    void LoadIdentity();
    void Translate(float x, float y, float z);
    void Rotate(float angle, float x, float y, float z);
    void Scale(float x, float y, float z);
    
    Matrix4 operator*(const Matrix4& other) const;
    Matrix4 Multiply(const Matrix4& other) const;
    
    // ДОБАВЬТЕ ЭТОТ МЕТОД
    float GetAspectRatio() const {
        // Простая реализация для получения соотношения сторон из матрицы проекции
        return m[5] != 0 ? m[0] / m[5] : 1.0f;
    }
};

} // namespace Revolt