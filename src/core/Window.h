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
    
    // Новые методы для управления вьюпортом
    void SetupViewportForScene();
    void ClearBlackBars();

    // Разрешения экрана
    static const int WIDTH_320 = 320;
    static const int HEIGHT_240 = 240;
    static const int WIDTH_512 = 512;
    static const int HEIGHT_384 = 384;
    static const int WIDTH_640 = 640;
    static const int HEIGHT_480 = 480;
    static const int WIDTH_800 = 800;
    static const int HEIGHT_600 = 600;

    int GetWidth() const { return m_renderWidth; }  // Разрешение рендеринга
    int GetHeight() const { return m_renderHeight; } // Разрешение рендеринга
    int GetScreenWidth() const { return m_screenWidth; } // Разрешение экрана
    int GetScreenHeight() const { return m_screenHeight; } // Разрешение экрана
    GLFWwindow* GetNativeWindow() const { return m_window; }

private:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    int m_renderWidth;   // Разрешение рендеринга (320x240, 512x384 и т.д.)
    int m_renderHeight;  // Разрешение рендеринга
    int m_screenWidth;   // Разрешение экрана (нативное)
    int m_screenHeight;  // Разрешение экрана
    std::string m_title;
    GLFWwindow* m_window;
};

} // namespace Revolt