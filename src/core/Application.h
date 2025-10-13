#pragma once
#include "Window.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "core/Scene.h"
#include "core/SceneLoader.h"
#include "graphics/TextRenderer.h"
#include <memory>

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

    // ВЫБЕРИТЕ НУЖНОЕ РАЗРЕШЕНИЕ РЕНДЕРИНГА:
    Window m_window{Window::WIDTH_800, Window::HEIGHT_600, "Revolt Engine"}; // 800x600
    // Window m_window{Window::WIDTH_640, Window::HEIGHT_480, "Revolt Engine"}; // 640x480
    // Window m_window{Window::WIDTH_512, Window::HEIGHT_384, "Revolt Engine"}; // 512x384  
    //Window m_window{Window::WIDTH_320, Window::HEIGHT_240, "Revolt Engine"}; // 320x240
    
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
};

} // namespace Revolt