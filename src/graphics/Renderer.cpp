#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <cmath>
#include <iostream>

namespace Revolt {

Renderer::Renderer() 
    : m_framebuffer(800, 600) {
}

void Renderer::Initialize(int width, int height) {
    // Инициализируем framebuffer с разрешением рендеринга
    m_framebuffer = Framebuffer(width, height);
    if (!m_framebuffer.Initialize()) {
        std::cerr << "Failed to initialize framebuffer!" << std::endl;
    }
    
    // Настройки OpenGL для 3D рендеринга
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // НАСТРОЙКИ СВЕТА
    float lightPos[] = {2.0f, 5.0f, 5.0f, 1.0f};
    float lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float ambientLight[] = {0.6f, 0.6f, 0.6f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    
    glEnable(GL_NORMALIZE);
    
    // Дополнительные настройки для лучших цветов
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Устанавливаем цвет очистки
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f); // Сине-зеленый
}

void Renderer::BeginFrame() {
    // Начинаем рендеринг в низком разрешении
    m_framebuffer.BeginRender();
    
    // Устанавливаем матрицу проекции для 3D сцены
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_camera.GetProjectionMatrix().m);
    
    // Переключаемся на MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::EndFrame() {
    // Заканчиваем рендеринг в низком разрешении и копируем в текстуру
    m_framebuffer.EndRender();
}

void Renderer::RenderToScreen(int screenWidth, int screenHeight) {
    // Рендерим текстуру низкого разрешения на экран
    m_framebuffer.RenderToScreen(screenWidth, screenHeight);
}

void Renderer::RenderMesh(Mesh& mesh, const Matrix4& transform) {
    // Отладочная информация
    static int renderCount = 0;
    if (renderCount++ % 60 == 0) { // Выводим каждые 60 кадров
        std::cout << "Rendering mesh, transform: " 
                  << transform.m[0] << ", " << transform.m[5] << ", " << transform.m[10] << std::endl;
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Загружаем матрицу вида камеры
    glLoadMatrixf(m_camera.GetViewMatrix().m);
    
    // Обновляем позиции источников света относительно камеры
    float lightPos[] = {5.0f, 5.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    // Применяем трансформацию объекта
    glMultMatrixf(transform.m);
    
    mesh.Render();
}

void Renderer::SetCamera(const Camera& camera) {
    m_camera = camera;
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    // Подавляем предупреждения
    (void)r; (void)g; (void)b; (void)a;
}

void Renderer::RenderMDLModel(MDLModel& model, const Matrix4& transform, int frame) {
    glPushMatrix();
    
    // Применяем трансформацию объекта - ИСПРАВЛЕННАЯ СТРОКА
    glMultMatrixf(transform.m);  // Используем прямое обращение к массиву
    
    // Рендерим MDL модель
    model.Render(frame);
    
    glPopMatrix();
}

} // namespace Revolt