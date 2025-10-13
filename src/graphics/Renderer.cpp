#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <cmath>

namespace Revolt {

Renderer::Renderer() {
}

void Renderer::Initialize(int width, int height) {
    // Используем параметры вместо подавления предупреждений
    
    (void)width;
    (void)height;
    
    glEnable(GL_DEPTH_TEST);
    // Убираем установку вьюпорта и очистку - это теперь делает Window
    
    // ВКЛЮЧАЕМ освещение
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Простые настройки света
    float lightPos[] = {2.0f, 5.0f, 5.0f, 1.0f};
    float lightColor[] = {0.8f, 0.8f, 0.8f, 1.0f};
    float ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    
    // Включаем нормализацию нормалей
    glEnable(GL_NORMALIZE);
}

void Renderer::RenderMesh(Mesh& mesh, const Matrix4& transform) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Загружаем матрицу вида камеры
    glLoadMatrixf(m_camera.GetViewMatrix().m);
    
    // Обновляем позиции источников света относительно камеры
    float lightPos[] = {5.0f, 5.0f, 5.0f, 1.0f};
    float lightPos2[] = {-3.0f, 3.0f, -3.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
    
    // Применяем трансформацию объекта
    glMultMatrixf(transform.m);
    
    mesh.Render();
}

void Renderer::BeginFrame() {
    // УБИРАЕМ очистку - это теперь делает Window
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Устанавливаем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_camera.GetProjectionMatrix().m);
    
    // Переключаемся на MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::EndFrame() {
    // Пока пусто
}

void Renderer::SetCamera(const Camera& camera) {
    m_camera = camera;
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    // Просто подавляем предупреждения, так как метод пока не используется
    (void)r;
    (void)g;
    (void)b;
    (void)a;
}

} // namespace Revolt