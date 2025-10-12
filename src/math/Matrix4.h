#pragma once

namespace Revolt {

class Matrix4 {
public:
    float m[16];

    Matrix4();
    
    static Matrix4 Identity();
    static Matrix4 Perspective(float fov, float aspect, float nearPlane, float farPlane);
    
    void LoadIdentity();
    void Translate(float x, float y, float z);
    void Rotate(float angle, float x, float y, float z);
    
    void Multiply(const Matrix4& other);
};

} // namespace Revolt