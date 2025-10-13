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

    // Получаем монитор и его видеорежим
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    // Устанавливаем версию OpenGL (совместимый профиль)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    
    // Создаем полноэкранное окно
    glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
    
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), primaryMonitor, nullptr);
    
    if (!m_window) {
        std::cerr << "Failed to create fullscreen window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    
    // Устанавливаем обработчик клавиш
    glfwSetKeyCallback(m_window, KeyCallback);
    
    // Включаем необходимые функции OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Fullscreen mode: " << m_width << "x" << m_height << std::endl;
    
    return true;
}

void Window::SetupViewportForScene() {
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
    
    // Вычисляем соотношение сторон
    float targetAspect = (float)m_width / (float)m_height;
    float screenAspect = (float)framebufferWidth / (float)framebufferHeight;
    
    int viewportWidth, viewportHeight;
    int viewportX, viewportY;
    
    if (screenAspect > targetAspect) {
        // Черные полосы по бокам
        viewportHeight = framebufferHeight;
        viewportWidth = (int)(framebufferHeight * targetAspect);
        viewportX = (framebufferWidth - viewportWidth) / 2;
        viewportY = 0;
    } else {
        // Черные полосы сверху и снизу
        viewportWidth = framebufferWidth;
        viewportHeight = (int)(framebufferWidth / targetAspect);
        viewportX = 0;
        viewportY = (framebufferHeight - viewportHeight) / 2;
    }
    
    // Устанавливаем вьюпорт для рендеринга сцены
    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
    
    // Очищаем ТОЛЬКО область вьюпорта цветом сцены
    glScissor(viewportX, viewportY, viewportWidth, viewportHeight);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

void Window::ClearBlackBars() {
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
    
    float targetAspect = (float)m_width / (float)m_height;
    float screenAspect = (float)framebufferWidth / (float)framebufferHeight;
    
    // Очищаем ВЕСЬ экран черным цветом
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
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