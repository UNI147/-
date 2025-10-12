#include "Camera.h"
#include <cmath>

namespace Revolt {

Camera::Camera() {
    m_projection = Matrix4::Identity();
    m_view = Matrix4::Identity();
}

void Camera::SetPerspective(float fov, float aspect, float nearPlane, float farPlane) {
    m_projection = Matrix4::Perspective(fov, aspect, nearPlane, farPlane);
}

void Camera::SetPosition(float x, float y, float z) {
    LookAt(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void Camera::LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ) {
    // Вызываем полную версию с вектором up по умолчанию
    LookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);
}

void Camera::LookAt(float eyeX, float eyeY, float eyeZ, 
        float centerX, float centerY, float centerZ,
        float upX, float upY, float upZ) {

    // Вычисляем направление взгляда
    float forward[3] = {
        centerX - eyeX,
        centerY - eyeY, 
        centerZ - eyeZ
    };

    // Нормализуем forward
    float length = std::sqrt(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]); // Исправлено: std::sqrt
    if (length > 0) {
        forward[0] /= length;
        forward[1] /= length;
        forward[2] /= length;
    }

    // Вычисляем right вектор (cross product up x forward)
    float up[3] = {upX, upY, upZ};
    float right[3] = {
        up[1]*forward[2] - up[2]*forward[1],
        up[2]*forward[0] - up[0]*forward[2],
        up[0]*forward[1] - up[1]*forward[0]
    };

    // Нормализуем right
    length = std::sqrt(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]); // Исправлено: std::sqrt
    if (length > 0) {
        right[0] /= length;
        right[1] /= length;
        right[2] /= length;
    }

    // Пересчитываем up (cross product forward x right)
    up[0] = forward[1]*right[2] - forward[2]*right[1];
    up[1] = forward[2]*right[0] - forward[0]*right[2];
    up[2] = forward[0]*right[1] - forward[1]*right[0];

    // Создаем матрицу вида
    m_view = Matrix4::Identity();

    // Устанавливаем компоненты
    m_view.m[0] = right[0]; m_view.m[4] = right[1]; m_view.m[8] = right[2];
    m_view.m[1] = up[0];    m_view.m[5] = up[1];    m_view.m[9] = up[2];
    m_view.m[2] = -forward[0]; m_view.m[6] = -forward[1]; m_view.m[10] = -forward[2];

    // Добавляем трансляцию
    m_view.m[12] = -right[0]*eyeX - right[1]*eyeY - right[2]*eyeZ;
    m_view.m[13] = -up[0]*eyeX - up[1]*eyeY - up[2]*eyeZ;
    m_view.m[14] = forward[0]*eyeX + forward[1]*eyeY + forward[2]*eyeZ;
}

} // namespace Revolt