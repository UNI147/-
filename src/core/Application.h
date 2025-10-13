#pragma once
#include "Window.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "core/Scene.h"
#include "core/SceneLoader.h"
#include "graphics/TextRenderer.h"
#include <memory>
#include <vector>

namespace Revolt {

class Application {
public:
    Application();
    ~Application();
    
    bool Initialize();
    void Run();
    void Shutdown();
    
    void PrintSceneInfo();

private:
    void Update(float deltaTime);
    void Render();
    void UpdateFPS(float deltaTime);
    void ToggleDebugInfo();
    void CycleResolution(); // Новый метод для переключения разрешения

    // Список доступных разрешений
    struct Resolution {
        int width;
        int height;
    };
    
    std::vector<Resolution> m_resolutions = {
        {Window::WIDTH_800, Window::HEIGHT_600},   // 800x600
        {Window::WIDTH_640, Window::HEIGHT_480},   // 640x480
        {Window::WIDTH_512, Window::HEIGHT_384},   // 512x384  
        {Window::WIDTH_320, Window::HEIGHT_240}    // 320x240
    };
    
    size_t m_currentResolutionIndex = 0; // Текущее разрешение
    
    Window m_window{m_resolutions[0].width, m_resolutions[0].height, "Revolt Engine"};
    Renderer m_renderer;
    Camera m_camera;
    Scene m_scene;
    TextRenderer m_textRenderer;
    bool m_isRunning;
    bool m_showDebugInfo;
    
    // Переменные для FPS
    float m_fps;
    float m_fpsAccumulator;
    int m_fpsFrames;
    float m_fpsRefreshTime;
    
    // Для обработки клавиши Tab
    bool m_tabPressed = false;
};

} // namespace Revolt