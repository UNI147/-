#include "Camera.h"

namespace Revolt {

Camera::Camera() {
    m_projection = Matrix4::Identity();
    m_view = Matrix4::Identity();
}

void Camera::SetPerspective(float fov, float aspect, float nearPlane, float farPlane) {
    m_projection = Matrix4::Perspective(fov, aspect, nearPlane, farPlane);
}

void Camera::SetPosition(float x, float y, float z) {
    m_view.Translate(-x, -y, -z);
}

void Camera::LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ) {
    // Вызываем полную версию с вектором up по умолчанию
    LookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);
}

void Camera::LookAt(float eyeX, float eyeY, float eyeZ, 
                   float centerX, float centerY, float centerZ,
                   float upX, float upY, float upZ) {
    // TODO: Реализовать полноценный LookAt
    // Временная реализация - просто устанавливаем позицию
    m_view = Matrix4::Identity();
    m_view.Translate(-eyeX, -eyeY, -eyeZ);
}

} // namespace Revolt