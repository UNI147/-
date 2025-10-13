#include "Application.h"
#include "core/ResourceManager.h"
#include "core/Scene.h"
#include "core/SceneLoader.h"
#include "graphics/TextRenderer.h" 
#include <iostream>

namespace Revolt {

Application::Application() 
    // Конструктор Window уже инициализирован в объявлении с выбранным разрешением
    : m_isRunning(false)
    , m_showDebugInfo(false)
    , m_fps(0.0f)
    , m_fpsAccumulator(0.0f)
    , m_fpsFrames(0)
    , m_fpsRefreshTime(0.0f) {
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
    
    // Инициализируем рендерер с РАЗРЕШЕНИЕМ РЕНДЕРИНГА
    m_renderer.Initialize(m_window.GetWidth(), m_window.GetHeight());
    
    // Инициализируем TextRenderer
    if (!m_textRenderer.Initialize()) {
        std::cerr << "Failed to initialize TextRenderer" << std::endl;
        return false;
    }
    
    // Устанавливаем разрешение рендеринга для TextRenderer
    m_textRenderer.SetRenderResolution(m_window.GetWidth(), m_window.GetHeight());
    m_textRenderer.SetWindowSize(m_window.GetScreenWidth(), m_window.GetScreenHeight());
    
    // Обновляем камеру для соотношения сторон РЕНДЕРИНГА
    float aspectRatio = (float)m_window.GetWidth() / (float)m_window.GetHeight();
    m_camera.SetPerspective(1.0472f, aspectRatio, 0.1f, 100.0f);
    
    // Загружаем сцену
    SceneLoader::LoadSceneFromFile("../../assets/demo_scene.json", m_scene, m_camera);

    // Выводим информацию для отладки
    PrintSceneInfo();
    std::cout << "Render resolution: " << m_window.GetWidth() << "x" << m_window.GetHeight() << std::endl;
    std::cout << "Screen resolution: " << m_window.GetScreenWidth() << "x" << m_window.GetScreenHeight() << std::endl;
    
    // Устанавливаем камеру в рендерер
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
        
        // Обновляем FPS
        UpdateFPS(deltaTime);
        
        // Очищаем черные полосы (области за пределами сцены)
        m_window.ClearBlackBars();
        
        // Настраиваем вьюпорт для сцены (центрируем изображение)
        m_window.SetupViewportForScene();
        
        Update(deltaTime);
        Render();
        m_window.SwapBuffers();
    }
}

void Application::Shutdown() {
    m_isRunning = false;
}

void Application::UpdateFPS(float deltaTime) {
    m_fpsAccumulator += deltaTime;
    m_fpsFrames++;
    
    // Обновляем FPS каждую секунду
    if (m_fpsAccumulator >= 1.0f) {
        m_fps = static_cast<float>(m_fpsFrames) / m_fpsAccumulator;
        m_fpsFrames = 0;
        m_fpsAccumulator = 0.0f;
    }
}

void Application::ToggleDebugInfo() {
    m_showDebugInfo = !m_showDebugInfo;
    std::cout << "Debug info: " << (m_showDebugInfo ? "ON" : "OFF") << std::endl;
}

void Application::Update(float deltaTime) {
    static float totalTime = 0.0f;
    totalTime += deltaTime;
    
    // Обработка клавиши "`"/"ё" для переключения отладочной информации
    static bool keyPressed = false;
    if (glfwGetKey(m_window.GetNativeWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
        if (!keyPressed) {
            ToggleDebugInfo();
            keyPressed = true;
        }
    } else {
        keyPressed = false;
    }
    
    const float rotationSpeed = 180.0f; // градусов в секунду для всех объектов
    
    const auto& objects = m_scene.GetObjects();
    for (size_t i = 0; i < objects.size(); ++i) {
        float rotation = totalTime * rotationSpeed;
        
        objects[i]->SetRotation(0.0f, rotation, 0.0f);
        objects[i]->UpdateTransform();
    }
}

void Application::Render() {
    // Рендерим 3D сцену в низком разрешении
    m_renderer.BeginFrame();
    
    // Рендерим все объекты сцены В НИЗКОМ РАЗРЕШЕНИИ
    const auto& objects = m_scene.GetObjects();
    for (const auto& obj : objects) {
        if (obj->GetMesh()) {
            m_renderer.RenderMesh(*obj->GetMesh(), obj->GetTransform());
        }
    }
    
    // РЕНДЕРИМ UI ТОЖЕ В НИЗКОМ РАЗРЕШЕНИИ
    if (m_showDebugInfo) {
        // Форматируем текст в нужном формате: "(разрешение)(FPS значение)"
        std::string resolutionInfo = "(" + std::to_string(m_window.GetWidth()) + "x" + 
                                std::to_string(m_window.GetHeight()) + ")";
        std::string fpsInfo = "(FPS " + std::to_string(static_cast<int>(m_fps)) + ")";
        std::string debugText = resolutionInfo + fpsInfo;

        // Позиционируем в ВЕРХНЕМ левом углу
        float textScale = 4.0f;
        float textX = 20.0f;
        float textY = 40.0f;

        // Рендерим текст через TextRenderer (только одну строку)
        m_textRenderer.RenderText(debugText, textX, textY, textScale);
    }
    
    m_renderer.EndFrame();
    
    // Рендерим низкое разрешение на экран (растягиваем без интерполяции)
    m_renderer.RenderToScreen(m_window.GetScreenWidth(), m_window.GetScreenHeight());
}

} // namespace Revolt