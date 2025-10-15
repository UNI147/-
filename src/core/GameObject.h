#pragma once
#include "../math/Matrix4.h"
#include "../graphics/Mesh.h"
#include "../graphics/MDLModel.h" // Добавляем include
#include <memory>

namespace Revolt {
    class GameObject {
    public:
        GameObject();
        
        void SetMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
        std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
        
        // Добавляем методы для MDL моделей
        void SetMDLModel(std::shared_ptr<MDLModel> model) { m_mdlModel = model; }
        std::shared_ptr<MDLModel> GetMDLModel() const { return m_mdlModel; }
        
        void SetMaterial(const Material& material) { 
            m_material = material; 
            ApplyMaterialToMesh(); // Автоматически применяем к мешу
        }
        const Material& GetMaterial() const { return m_material; }
        
        // Для анимации MDL моделей
        void SetCurrentFrame(int frame) { m_currentFrame = frame; }
        int GetCurrentFrame() const { return m_currentFrame; }
        
        void SetPosition(float x, float y, float z);
        void SetRotation(float x, float y, float z);
        void SetScale(float x, float y, float z);
        
        const Matrix4& GetTransform() const { return m_transform; }
        void UpdateTransform();
    
        void Update(float deltaTime);
        
        float GetPositionX() const { return m_position[0]; }
        float GetPositionY() const { return m_position[1]; }
        float GetPositionZ() const { return m_position[2]; }
        
        float GetRotationX() const { return m_rotation[0]; }
        float GetRotationY() const { return m_rotation[1]; }
        float GetRotationZ() const { return m_rotation[2]; }
        
    private:
        void ApplyMaterialToMesh(); // Применяет материал к мешу
        
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<MDLModel> m_mdlModel; // Новая переменная для MDL моделей
        Material m_material;
        
        float m_position[3];
        float m_rotation[3]; // В градусах, для простоты
        float m_scale[3];
        
        Matrix4 m_transform;
        bool m_transformDirty;
        
        int m_currentFrame; // Текущий кадр анимации для MDL моделей
    };
}