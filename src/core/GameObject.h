#pragma once
#include "../math/Matrix4.h"
#include "../graphics/Mesh.h"
#include <memory>

namespace Revolt {
    class GameObject {
    public:
        GameObject();
        
        void SetMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
        std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
        
        void SetMaterial(const Material& material) { 
            m_material = material; 
            ApplyMaterialToMesh(); // Автоматически применяем к мешу
        }
        const Material& GetMaterial() const { return m_material; }
        
        void SetPosition(float x, float y, float z);
        void SetRotation(float x, float y, float z);
        void SetScale(float x, float y, float z);
        
        const Matrix4& GetTransform() const { return m_transform; }
        void UpdateTransform();

        float GetPositionX() const { return m_position[0]; }
        float GetPositionY() const { return m_position[1]; }
        float GetPositionZ() const { return m_position[2]; }
        
        float GetRotationX() const { return m_rotation[0]; }
        float GetRotationY() const { return m_rotation[1]; }
        float GetRotationZ() const { return m_rotation[2]; }
        
    private:
        void ApplyMaterialToMesh(); // Применяет материал к мешу
        
        std::shared_ptr<Mesh> m_mesh;
        Material m_material;
        
        float m_position[3];
        float m_rotation[3]; // В градусах, для простоты
        float m_scale[3];
        
        Matrix4 m_transform;
        bool m_transformDirty;
    };
}