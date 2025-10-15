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
        
        // Загрузка камеры с проверками
        if (!sceneData.contains("camera")) {
            std::cerr << "No camera data in scene file" << std::endl;
            CreateDemoScene(scene, camera);
            return false;
        }
        
        const auto& camData = sceneData["camera"];
        
        // Используем фиксированное соотношение сторон или из данных
        float aspect = camData.contains("aspect") ? camData["aspect"] : (16.0f / 9.0f);
        float fov = camData.contains("fov") ? camData["fov"] : 1.0472f;
        float nearPlane = camData.contains("near") ? camData["near"] : 0.1f;
        float farPlane = camData.contains("far") ? camData["far"] : 100.0f;
        
        camera.SetPerspective(fov, aspect, nearPlane, farPlane);
        
        // Позиция и lookAt с проверками
        if (camData.contains("position") && camData.contains("lookAt")) {
            const auto& pos = camData["position"];
            const auto& lookAt = camData["lookAt"];
            
            if (pos.size() >= 3 && lookAt.size() >= 3) {
                camera.LookAt(pos[0], pos[1], pos[2], lookAt[0], lookAt[1], lookAt[2]);
            } else {
                camera.LookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f);
            }
        } else {
            camera.LookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f);
        }
        
        // Загрузка объектов с проверками
        if (!sceneData.contains("objects")) {
            std::cerr << "No objects in scene file" << std::endl;
            CreateDemoScene(scene, camera);
            return false;
        }
        
        auto& resourceManager = ResourceManager::GetInstance();
        for (const auto& objData : sceneData["objects"]) {
            auto obj = scene.CreateGameObject();
            std::shared_ptr<Mesh> mesh;
            
            // Проверяем обязательные поля
            if (!objData.contains("type")) {
                std::cerr << "Object missing type" << std::endl;
                continue;
            }
            
            std::string type = objData["type"];
            
            if (type == "Pyramid") {
                const auto& params = objData.contains("parameters") ? objData["parameters"] : json::object();
                float base = params.contains("base") ? params["base"] : 1.0f;
                float height = params.contains("height") ? params["height"] : 1.5f;
                mesh = resourceManager.LoadMesh("Pyramid", base, height);
            } else if (type == "Cube") {
                const auto& params = objData.contains("parameters") ? objData["parameters"] : json::object();
                float size = params.contains("size") ? params["size"] : 0.8f;
                mesh = resourceManager.LoadMesh("Cube", size);
            } else if (type == "Torus") {
                const auto& params = objData.contains("parameters") ? objData["parameters"] : json::object();
                float majorRadius = params.contains("majorRadius") ? params["majorRadius"] : 1.0f;
                float minorRadius = params.contains("minorRadius") ? params["minorRadius"] : 0.3f;
                int majorSegments = params.contains("majorSegments") ? params["majorSegments"] : 16;
                int minorSegments = params.contains("minorSegments") ? params["minorSegments"] : 8;
                mesh = resourceManager.LoadMesh("Torus", majorRadius, minorRadius, majorSegments, minorSegments);
            } else if (type == "MDLModel") {
                // Загрузка MDL модели
                if (objData.contains("filename")) {
                    std::string filename = objData["filename"];
                    auto mdlModel = resourceManager.LoadMDLModel(filename);
                    if (mdlModel) {
                        obj->SetMDLModel(mdlModel);
                        std::cout << "Loaded MDL model: " << filename << std::endl;
                    } else {
                        std::cerr << "Failed to load MDL model: " << filename << std::endl;
                    }
                }
            } else {
                std::cerr << "Unknown object type: " << type << std::endl;
                continue;
            }
            
            if (mesh || obj->GetMDLModel()) {
                if (mesh) {
                    obj->SetMesh(mesh);
                }
                
                // Загрузка материала с проверками
                Material material(1.0f, 1.0f, 1.0f); // Белый по умолчанию
                if (objData.contains("material") && objData["material"].contains("color")) {
                    const auto& color = objData["material"]["color"];
                    if (color.size() >= 3) {
                        float r = color[0];
                        float g = color[1];
                        float b = color[2];
                        float a = (color.size() >= 4) ? color[3] : 1.0f;
                        material = Material(r, g, b, a);
                    }
                }
                obj->SetMaterial(material);
            
                // ДЕБАГ: выводим ВСЕ параметры
                std::cout << "Loaded " << type << " - ";
                if (type == "Pyramid") {
                    const auto& params = objData["parameters"];
                    std::cout << "base: " << params["base"] << ", height: " << params["height"];
                } else if (type == "Cube") {
                    const auto& params = objData["parameters"];
                    std::cout << "size: " << params["size"];
                } else if (type == "Torus") {
                    const auto& params = objData["parameters"];
                    std::cout << "majorRadius: " << params["majorRadius"] << ", minorRadius: " << params["minorRadius"];
                } else if (type == "MDLModel") {
                    std::cout << "filename: " << objData["filename"];
                }
                std::cout << ", color: (" << material.GetR() << ", " << material.GetG() << ", " << material.GetB() << ", " << material.GetA()
                          << ")" << std::endl;
            }
            
            // Загрузка трансформации с проверками
            float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
            float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;
            float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
            
            if (objData.contains("position") && objData["position"].size() >= 3) {
                const auto& position = objData["position"];
                posX = position[0]; posY = position[1]; posZ = position[2];
            }
            
            if (objData.contains("rotation") && objData["rotation"].size() >= 3) {
                const auto& rotation = objData["rotation"];
                rotX = rotation[0]; rotY = rotation[1]; rotZ = rotation[2];
            }
            
            if (objData.contains("scale") && objData["scale"].size() >= 3) {
                const auto& scale = objData["scale"];
                scaleX = scale[0]; scaleY = scale[1]; scaleZ = scale[2];
            }
            
            obj->SetPosition(posX, posY, posZ);
            
            // Для тора - добавляем начальный поворот на 90 градусов по X
            if (type == "Torus") {
                obj->SetRotation(rotX + 90.0f, rotY, rotZ);
            } else {
                obj->SetRotation(rotX, rotY, rotZ);
            }
            
            obj->SetScale(scaleX, scaleY, scaleZ);
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

    // MDL модель для тестирования
    auto mdlObj = scene.CreateGameObject();
    auto mdlModel = ResourceManager::GetInstance().LoadMDLModel("assets/player.mdl");
    if (mdlModel) {
        mdlObj->SetMDLModel(mdlModel);
        mdlObj->SetPosition(0.0f, -1.0f, 0.0f);
        mdlObj->UpdateTransform();
        std::cout << "Loaded MDL model for demo scene" << std::endl;
    } else {
        std::cout << "MDL model not available, using fallback objects" << std::endl;
    }
}

} // namespace Revolt