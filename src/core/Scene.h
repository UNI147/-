#pragma once
#include "GameObject.h"
#include <vector>
#include <memory>

namespace Revolt {
    class Scene {
    public:
        Scene();
        
        GameObject* CreateGameObject();
        void RemoveGameObject(GameObject* object);
        
        const std::vector<std::unique_ptr<GameObject>>& GetObjects() const { return m_objects; }
        
        void Update(float deltaTime); // Для анимаций и логики
        
    private:
        std::vector<std::unique_ptr<GameObject>> m_objects;
    };
}