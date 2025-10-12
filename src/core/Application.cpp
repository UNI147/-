#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <GLFW/glfw3.h>
#include "Application.h"
#include <iostream>

namespace Revolt {

Application::Application() 
    : m_window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Revolt Engine")
    , m_isRunning(false) {
}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    if (!m_window.Initialize()) {
        return false;
    }
    
    m_renderer.Initialize(m_window.GetWidth(), m_window.GetHeight());
    
    // Настраиваем камеру
    m_camera.SetPerspective(1.0472f, 
                           (float)m_window.GetWidth()/m_window.GetHeight(), 
                           0.1f, 100.0f);
    m_camera.SetPosition(0.0f, 0.0f, 3.0f);
    m_camera.LookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    m_renderer.SetCamera(m_camera);
    
    // Создаем меш пирамиды
    m_pyramidMesh.CreatePyramid();
    
    m_isRunning = true;
    return true;
}

void Application::Run() {
    double lastTime = glfwGetTime();
    
    while (m_isRunning && !m_window.ShouldClose()) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        
        // Ограничение FPS (~72 кадра в секунду)
        if (deltaTime < 1.0/72.0) {
            continue;
        }
        
        lastTime = currentTime;
        
        Update(deltaTime);
        Render();
        
        m_window.SwapBuffers();
        m_window.PollEvents();
    }
}

void Application::Shutdown() {
    m_isRunning = false;
    glfwTerminate();
}

void Application::Update(float deltaTime) {
    // Логика обновления - пока пустая
}

void Application::Render() {
    m_renderer.BeginFrame();
    
    // Создаем матрицу преобразования для вращения
    Matrix4 transform = Matrix4::Identity();
    transform.Rotate(static_cast<float>(glfwGetTime()) * 50.0f, 0.0f, 1.0f, 0.0f);
    
    // Рендерим пирамиду
    m_renderer.RenderMesh(m_pyramidMesh, transform);
    
    m_renderer.EndFrame();
}

} // namespace Revolt