#pragma once
#include "Window.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "core/Scene.h"
#include "core/SceneLoader.h"
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

    Window m_window;
    Renderer m_renderer;
    Camera m_camera;
    Scene m_scene;
    bool m_isRunning;
    
    static const int DEFAULT_WIDTH = 800;
    static const int DEFAULT_HEIGHT = 600;
};

} // namespace Revolt