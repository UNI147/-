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

    Window m_window;
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
    
    static const int DEFAULT_WIDTH = 800;
    static const int DEFAULT_HEIGHT = 600;
};

} // namespace Revolt