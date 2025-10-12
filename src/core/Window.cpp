#include "Window.h"
#include <iostream>

namespace Revolt {

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr) {
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

    // Устанавливаем версию OpenGL (совместимый профиль)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    
    // ЗАПРЕЩАЕМ изменение размера окна
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    glfwWindowHint(GLFW_REFRESH_RATE, 72);
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    
    if (!m_window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    
    // Включаем необходимые функции OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    return true;
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