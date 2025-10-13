#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Application.h"
#include "core/ResourceManager.h"
#include "core/Scene.h"
#include "core/SceneLoader.h"
#include <iostream>

namespace Revolt {

Application::Application() 
    : m_window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Revolt Engine")
    , m_isRunning(false) {
}

Application::~Application() {
}

void Application::PrintSceneInfo() {
    std::cout << "Scene objects count: " << m_scene.GetObjects().size() << std::endl;
    for (const auto& obj : m_scene.GetObjects()) {
        std::cout << "Object at position: " 
                  << obj->GetPositionX() << ", " 
                  << obj->GetPositionY() << ", " 
                  << obj->GetPositionZ() << std::endl;
    }
}

bool Application::Initialize() {
    if (!m_window.Initialize()) {
        return false;
    }
    
    m_renderer.Initialize(m_window.GetWidth(), m_window.GetHeight());
    
    // Загружаем сцену
    SceneLoader::LoadSceneFromFile("../../assets/demo_scene.json", m_scene, m_camera);

    
    // Выводим информацию о сцене для отладки
    PrintSceneInfo();
    
    // Устанавливаем камеру в рендерер ПОСЛЕ загрузки
    m_renderer.SetCamera(m_camera);
    
    m_isRunning = true;
    return true;
}

void Application::Run() {
    double lastTime = glfwGetTime();
    
    while (!m_window.ShouldClose() && m_isRunning) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        
        m_window.PollEvents();
        
        // Очищаем черные полосы
        m_window.ClearBlackBars();
        
        // Настраиваем вьюпорт для сцены
        m_window.SetupViewportForScene();
        
        Update(deltaTime);
        Render();
        m_window.SwapBuffers();
    }
}

void Application::Shutdown() {
    m_isRunning = false;
}

void Application::Update(float deltaTime) {
    static float totalTime = 0.0f;
    totalTime += deltaTime;
    
    const float rotationSpeed = 180.0f; // градусов в секунду для всех объектов
    
    const auto& objects = m_scene.GetObjects();
    for (size_t i = 0; i < objects.size(); ++i) {
        float rotation = totalTime * rotationSpeed;
        
        objects[i]->SetRotation(0.0f, rotation, 0.0f);
        objects[i]->UpdateTransform();
    }
}

void Application::Render() {
    m_renderer.BeginFrame();
    
    // Рендерим все объекты сцены
    const auto& objects = m_scene.GetObjects();
    for (const auto& obj : objects) {
        if (obj->GetMesh()) {
            m_renderer.RenderMesh(*obj->GetMesh(), obj->GetTransform());
        }
    }
    
    m_renderer.EndFrame();
}

} // namespace Revolt