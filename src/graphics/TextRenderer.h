#pragma once
#include <string>
#include <GLFW/glfw3.h>

namespace Revolt {

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();
    
    bool Initialize();
    void RenderText(const std::string& text, float x, float y, float scale = 1.0f);
    void SetWindowSize(int width, int height);

private:
    void DrawDigit(char digit, float x, float y, float scale);
    void DrawLetter(char letter, float x, float y, float scale);
    void DrawSymbol(char symbol, float x, float y, float scale);
    
    int m_windowWidth;
    int m_windowHeight;
    bool m_initialized;
};

} // namespace Revolt