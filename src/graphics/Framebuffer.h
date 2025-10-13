#pragma once
#include <GLFW/glfw3.h>

namespace Revolt {

class Framebuffer {
public:
    Framebuffer(int width, int height);
    ~Framebuffer();
    
    bool Initialize();
    void BeginRender();
    void EndRender();
    void RenderToScreen(int screenWidth, int screenHeight);
    
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    int m_width;
    int m_height;
    GLuint m_textureID;
};

} // namespace Revolt