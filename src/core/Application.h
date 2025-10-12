#pragma once
#include "Window.h"
#include "../graphics/Renderer.h"
#include "../graphics/Mesh.h"
#include "../graphics/Camera.h"
#include "../math/Matrix4.h"

namespace Revolt {

class Application {
public:
    Application();
    ~Application();
    
    bool Initialize();
    void Run();
    void Shutdown();

private:
    void Update(float deltaTime);
    void Render();

    Window m_window;
    Renderer m_renderer;
    Camera m_camera;
    Mesh m_pyramidMesh;
    bool m_isRunning;
    
    // Сохраненные соотношения сторон из комментариев
    static const int DEFAULT_WIDTH = 800;
    static const int DEFAULT_HEIGHT = 600;
};

} // namespace Revolt