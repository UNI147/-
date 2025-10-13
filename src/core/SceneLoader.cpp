#include "SceneLoader.h"
#include "Scene.h"
#include "Camera.h"
#include "ResourceManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Revolt {

bool SceneLoader::LoadSceneFromFile(const std::string& filepath, Scene& scene, Camera& camera) {
    std::cout << "Loading scene from: " << filepath << std::endl;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open scene file: " << filepath << std::endl;
        CreateDemoScene(scene, camera);
        return false;
    }
    
    try {
        json sceneData = json::parse(file);
        
        // Загрузка камеры - УПРОЩЕННАЯ ВЕРСИЯ
        const auto& camData = sceneData["camera"];
        
        // Используем фиксированное соотношение сторон или из данных
        float aspect = camData.contains("aspect") ? camData["aspect"] : (16.0f / 9.0f);
        
        camera.SetPerspective(camData["fov"], aspect, camData["near"], camData["far"]);
        
        const auto& pos = camData["position"];
        const auto& lookAt = camData["lookAt"];
        camera.LookAt(pos[0], pos[1], pos[2], lookAt[0], lookAt[1], lookAt[2]);
        
        // Загрузка объектов
        auto& resourceManager = ResourceManager::GetInstance();
        for (const auto& objData : sceneData["objects"]) {
            auto obj = scene.CreateGameObject();
            std::shared_ptr<Mesh> mesh;
            
            // Загрузка меша
            const auto& params = objData["parameters"];
            std::string type = objData["type"];
            
            if (type == "Pyramid") {
                mesh = resourceManager.LoadMesh("Pyramid", params["base"], params["height"]);
            } else if (type == "Cube") {
                mesh = resourceManager.LoadMesh("Cube", params["size"]);
            } else if (type == "Torus") {
                mesh = resourceManager.LoadMesh("Torus", params["majorRadius"], params["minorRadius"], 
                                               params["majorSegments"], params["minorSegments"]);
            }
            
            if (mesh) {
                obj->SetMesh(mesh);
                
                // Загрузка материала
                const auto& color = objData["material"]["color"];
                Material material(color[0], color[1], color[2], color[3]);
                obj->SetMaterial(material);
            
                // ДЕБАГ: выводим ВСЕ параметры
                std::cout << "Loaded " << type << " - ";
                if (type == "Pyramid") {
                    std::cout << "base: " << params["base"] << ", height: " << params["height"];
                } else if (type == "Cube") {
                    std::cout << "size: " << params["size"];
                } else if (type == "Torus") {
                    std::cout << "majorRadius: " << params["majorRadius"] << ", minorRadius: " << params["minorRadius"];
                }
                std::cout << ", color: (" << color[0] << ", " << color[1] << ", " << color[2] << ", " << color[3] 
                          << ")" << std::endl;
            }
            
            // Загрузка трансформации
            const auto& position = objData["position"];
            const auto& rotation = objData["rotation"]; 
            const auto& scale = objData["scale"];
            
            obj->SetPosition(position[0], position[1], position[2]);
            
            // Для тора - добавляем начальный поворот на 90 градусов по X
            if (type == "Torus") {
                obj->SetRotation(rotation[0] + 90.0f, rotation[1], rotation[2]);
            } else {
                obj->SetRotation(rotation[0], rotation[1], rotation[2]);
            }
            
            obj->SetScale(scale[0], scale[1], scale[2]);
            obj->UpdateTransform();
        }
        
        std::cout << "Scene loaded successfully: " << sceneData["objects"].size() << " objects" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading scene: " << e.what() << std::endl;
        CreateDemoScene(scene, camera);
        return false;
    }
}

void SceneLoader::CreateDemoScene(Scene& scene, Camera& camera) {
    // Настраиваем камеру с соотношением сторон 16:9
    camera.SetPerspective(1.0472f, 16.0f/9.0f, 0.1f, 100.0f);
    camera.LookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f);
    
    auto& resourceManager = ResourceManager::GetInstance();
    
    // Красная пирамида
    auto pyramidObj = scene.CreateGameObject();
    pyramidObj->SetMesh(resourceManager.LoadMesh("Pyramid", 1.0f, 1.5f));
    pyramidObj->SetMaterial(Material(1.0f, 0.0f, 0.0f));
    pyramidObj->SetPosition(-1.5f, 0.0f, 0.0f);
    pyramidObj->UpdateTransform();
    
    // Зеленый куб
    auto cubeObj = scene.CreateGameObject();
    cubeObj->SetMesh(resourceManager.LoadMesh("Cube", 0.8f));
    cubeObj->SetMaterial(Material(0.0f, 1.0f, 0.0f, 0.7f));
    cubeObj->SetPosition(0.0f, 0.0f, 0.0f);
    cubeObj->UpdateTransform();
    
    // Желтый тор (с поворотом на 90 градусов по X)
    auto torusObj = scene.CreateGameObject();
    torusObj->SetMesh(resourceManager.LoadMesh("Torus", 1.0f, 0.3f, 16, 8));
    torusObj->SetMaterial(Material(1.0f, 1.0f, 0.0f));
    torusObj->SetPosition(1.5f, 0.0f, 0.0f);
    torusObj->SetRotation(90.0f, 0.0f, 0.0f); // Поворот на 90 градусов по X
    torusObj->UpdateTransform();
}

} // namespace Revolt