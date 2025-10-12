#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace Revolt {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool Initialize();
    void SwapBuffers();
    void PollEvents();
    bool ShouldClose() const;
    void Close();

    // Разрешения экрана
    static const int WIDTH_320 = 320;
    static const int HEIGHT_240 = 240;
    static const int WIDTH_512 = 512;
    static const int HEIGHT_384 = 384;
    static const int WIDTH_640 = 640;
    static const int HEIGHT_480 = 480;
    static const int WIDTH_800 = 800;
    static const int HEIGHT_600 = 600;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    GLFWwindow* GetNativeWindow() const { return m_window; }

private:
    int m_width;
    int m_height;
    std::string m_title;
    GLFWwindow* m_window;
};

} // namespace Revolt