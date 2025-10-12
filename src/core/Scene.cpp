#include "Scene.h"

namespace Revolt {

Scene::Scene() {
}

GameObject* Scene::CreateGameObject() {
    m_objects.push_back(std::make_unique<GameObject>());
    return m_objects.back().get();
}

void Scene::RemoveGameObject(GameObject* object) {
    m_objects.erase(
        std::remove_if(m_objects.begin(), m_objects.end(),
            [object](const std::unique_ptr<GameObject>& obj) {
                return obj.get() == object;
            }),
        m_objects.end()
    );
}

void Scene::Update(float deltaTime) {
    // Для будущей логики обновления сцены
}

} // namespace Revolt