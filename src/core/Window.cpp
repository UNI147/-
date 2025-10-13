#include "Window.h"
#include <iostream>

namespace Revolt {

Window::Window(int width, int height, const std::string& title)
    : m_renderWidth(width), m_renderHeight(height), m_title(title), m_window(nullptr) {
    // Разрешение рендеринга устанавливается через конструктор (320x240, 512x384 и т.д.)
    m_screenWidth = 0;
    m_screenHeight = 0;
}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
}

bool Window::Initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Получаем монитор и его видеорежим
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    // Сохраняем нативное разрешение экрана
    m_screenWidth = videoMode->width;
    m_screenHeight = videoMode->height;

    // Устанавливаем версию OpenGL (совместимый профиль)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    
    // Отключаем масштабирование и сглаживание
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
    
    // Создаем полноэкранное окно в нативном разрешении
    m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, m_title.c_str(), primaryMonitor, nullptr);
    
    if (!m_window) {
        std::cerr << "Failed to create fullscreen window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    
    // Устанавливаем обработчик клавиш
    glfwSetKeyCallback(m_window, KeyCallback);
    
    // Настройки OpenGL для точного рендеринга
    glEnable(GL_DEPTH_TEST);
    
    // Устанавливаем начальный вьюпорт
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Screen resolution: " << m_screenWidth << "x" << m_screenHeight << std::endl;
    std::cout << "Render resolution: " << m_renderWidth << "x" << m_renderHeight << std::endl;
    
    return true;
}

void Window::SetupViewportForScene() {
    // Больше не нужно - теперь рендеринг управляется через FBO
    
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Подавляем предупреждения для неиспользуемых параметров
    (void)scancode;
    (void)mods;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Window::ClearBlackBars() {
    // Больше не нужно - FBO сам очищает экран
    
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::PollEvents() {
    glfwPollEvents();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::Close() {
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

} // namespace Revolt