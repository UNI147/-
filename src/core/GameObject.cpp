#include "GameObject.h"

namespace Revolt {

GameObject::GameObject() 
    : m_position{0.0f, 0.0f, 0.0f}
    , m_rotation{0.0f, 0.0f, 0.0f}
    , m_scale{1.0f, 1.0f, 1.0f}
    , m_transform(Matrix4::Identity())
    , m_transformDirty(true) {
}

void GameObject::SetPosition(float x, float y, float z) {
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
    m_transformDirty = true;
}

void GameObject::SetRotation(float x, float y, float z) {
    m_rotation[0] = x;
    m_rotation[1] = y;
    m_rotation[2] = z;
    m_transformDirty = true;
}

void GameObject::SetScale(float x, float y, float z) {
    m_scale[0] = x;
    m_scale[1] = y;
    m_scale[2] = z;
    m_transformDirty = true;
}

void GameObject::UpdateTransform() {
    if (m_transformDirty) {
        // Начинаем с единичной матрицы
        m_transform = Matrix4::Identity();
        
        // ПРАВИЛЬНЫЙ порядок трансформаций:
        // 1. Масштабирование (первым)
        m_transform.Scale(m_scale[0], m_scale[1], m_scale[2]);
        
        // 2. Вращение (в градусах)
        m_transform.Rotate(m_rotation[0], 1.0f, 0.0f, 0.0f); // X
        m_transform.Rotate(m_rotation[1], 0.0f, 1.0f, 0.0f); // Y  
        m_transform.Rotate(m_rotation[2], 0.0f, 0.0f, 1.0f); // Z
        
        // 3. Позиция (последним)
        m_transform.Translate(m_position[0], m_position[1], m_position[2]);
        
        m_transformDirty = false;
    }
}

// ДОБАВЛЯЕМ метод для применения материала к мешу
void GameObject::ApplyMaterialToMesh() {
    if (m_mesh) {
        m_mesh->SetMaterial(m_material);
    }
}

} // namespace Revolt