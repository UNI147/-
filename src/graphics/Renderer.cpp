#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <GLFW/glfw3.h>
#include "Renderer.h"

namespace Revolt {

Renderer::Renderer() {
}

void Renderer::Initialize(int width, int height) {
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Renderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Устанавливаем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_camera.GetProjectionMatrix().m);
    
    // Устанавливаем матрицу вида
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_camera.GetViewMatrix().m);
}

void Renderer::EndFrame() {
    // Пока пусто - для будущего использования
}

void Renderer::RenderMesh(Mesh& mesh, const Matrix4& transform) {
    glPushMatrix();
    glMultMatrixf(transform.m);
    mesh.Render();
    glPopMatrix();
}

void Renderer::SetCamera(const Camera& camera) {
    m_camera = camera;
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

} // namespace Revolt