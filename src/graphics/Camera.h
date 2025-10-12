#pragma once
#include "../math/Matrix4.h"

namespace Revolt {

class Camera {
public:
    Camera();
    
    void SetPerspective(float fov, float aspect, float nearPlane, float farPlane);
    void SetPosition(float x, float y, float z);
    void LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ);
    void LookAt(float eyeX, float eyeY, float eyeZ, 
                float centerX, float centerY, float centerZ,
                float upX, float upY, float upZ); // Добавьте эту перегрузку
    
    const Matrix4& GetProjectionMatrix() const { return m_projection; }
    const Matrix4& GetViewMatrix() const { return m_view; }

private:
    Matrix4 m_projection;
    Matrix4 m_view;
};

} // namespace Revolt