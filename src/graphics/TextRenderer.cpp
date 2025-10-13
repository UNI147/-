#include "TextRenderer.h"
#include <iostream>

namespace Revolt {

TextRenderer::TextRenderer() 
    : m_windowWidth(800), m_windowHeight(600), m_initialized(false) {
}

TextRenderer::~TextRenderer() {
}

bool TextRenderer::Initialize() {
    m_initialized = true;
    return true;
}

void TextRenderer::SetWindowSize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
}

void TextRenderer::RenderText(const std::string& text, float x, float y, float scale) {
    if (!m_initialized) return;
    
    // Сохраняем текущие настройки OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    
    // Устанавливаем 2D проекцию (координаты: 0,0 в левом нижнем углу ВСЕГО ЭКРАНА)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_windowWidth, 0, m_windowHeight, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Настройки для 2D рендеринга
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);
    
    // Рисуем фон (прямоугольник под текстом)
    float bgWidth = text.length() * 12 * scale;
    float bgHeight = 18 * scale;
    
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(x - 5, y - 3);
    glVertex2f(x + bgWidth, y - 3);
    glVertex2f(x + bgWidth, y + bgHeight);
    glVertex2f(x - 5, y + bgHeight);
    glEnd();
    
    // Цвет текста
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5f);
    
    float currentX = x;
    
    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        
        if (c == ' ') {
            currentX += 8 * scale;
            continue;
        }
        
        // Рисуем символы
        if (c >= '0' && c <= '9') {
            DrawDigit(c, currentX, y, scale);
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            DrawLetter(c, currentX, y, scale);
        } else {
            DrawSymbol(c, currentX, y, scale);
        }
        
        currentX += 12 * scale;
    }
    
    // Восстанавливаем настройки
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

void TextRenderer::DrawDigit(char digit, float x, float y, float scale) {
    float w = 8 * scale;
    float h = 12 * scale;
    
    switch (digit) {
        case '0':
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y); glVertex2f(x + w, y); 
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glEnd();
            break;
        case '1':
            glBegin(GL_LINES);
            glVertex2f(x + w/2, y); glVertex2f(x + w/2, y + h);
            glEnd();
            break;
        case '2':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x + w, y + h);
            glVertex2f(x + w, y + h/2); glVertex2f(x, y + h/2);
            glVertex2f(x, y); glVertex2f(x + w, y);
            glEnd();
            break;
        case '3':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x + w, y + h);
            glVertex2f(x + w, y); glVertex2f(x, y);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w*0.7f, y + h/2);
            glEnd();
            break;
        case '4':
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glVertex2f(x + w, y + h/2); glVertex2f(x + w, y);
            glVertex2f(x, y + h); glVertex2f(x, y + h/2);
            glEnd();
            break;
        case '5':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glVertex2f(x + w, y); glVertex2f(x, y);
            glEnd();
            break;
        case '6':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glVertex2f(x, y); glVertex2f(x + w, y);
            glVertex2f(x + w, y + h/2); glVertex2f(x, y + h/2);
            glEnd();
            break;
        case '7':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x + w, y + h);
            glVertex2f(x + w, y);
            glEnd();
            break;
        case '8':
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y); glVertex2f(x + w, y); 
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glEnd();
            break;
        case '9':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x + w, y);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glEnd();
            break;
    }
}

void TextRenderer::DrawLetter(char letter, float x, float y, float scale) {
    float w = 8 * scale;
    float h = 12 * scale;
    
    switch (letter) {
        case 'a': case 'A':
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y); glVertex2f(x + w, y);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glEnd();
            break;
        case 'b': case 'B':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w*0.7f, y + h); glVertex2f(x + w, y + h*0.7f);
            glVertex2f(x + w*0.7f, y + h/2); glVertex2f(x + w, y + h*0.3f);
            glVertex2f(x + w*0.7f, y); glVertex2f(x, y);
            glEnd();
            break;
        case 'c': case 'C':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glVertex2f(x, y); glVertex2f(x + w, y);
            glEnd();
            break;
        case 'd': case 'D':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w*0.7f, y + h); glVertex2f(x + w, y + h/2);
            glVertex2f(x + w*0.7f, y); glVertex2f(x, y);
            glEnd();
            break;
        case 'e': case 'E':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y); glVertex2f(x, y);
            glVertex2f(x, y + h); glVertex2f(x + w, y + h);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w*0.7f, y + h/2);
            glEnd();
            break;
        case 'f': case 'F':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w, y + h);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w*0.7f, y + h/2);
            glEnd();
            break;
        case 'g': case 'G':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glVertex2f(x, y); glVertex2f(x + w, y);
            glVertex2f(x + w, y + h/2); glVertex2f(x + w/2, y + h/2);
            glEnd();
            break;
        case 'h': case 'H':
            glBegin(GL_LINES);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w, y); glVertex2f(x + w, y + h);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glEnd();
            break;
        case 'i': case 'I':
            glBegin(GL_LINES);
            glVertex2f(x + w/2, y); glVertex2f(x + w/2, y + h);
            glEnd();
            break;
        case 'j': case 'J':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y + h); glVertex2f(x + w/2, y + h);
            glVertex2f(x + w/2, y); 
            glEnd();
            break;
        case 'k': case 'K':
            glBegin(GL_LINES);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h/2);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y);
            glEnd();
            break;
        case 'l': case 'L':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x, y);
            glVertex2f(x + w, y);
            glEnd();
            break;
        case 'm': case 'M':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w, y + h);
            glVertex2f(x + w, y);
            glEnd();
            break;
        case 'n': case 'N':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w, y); glVertex2f(x + w, y + h);
            glEnd();
            break;
        case 'o': case 'O':
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y); glVertex2f(x + w, y); 
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glEnd();
            break;
        case 'p': case 'P':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w, y + h); glVertex2f(x + w, y + h/2);
            glVertex2f(x, y + h/2);
            glEnd();
            break;
        case 'q': case 'Q':
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y); glVertex2f(x + w, y); 
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w, y + h);
            glEnd();
            break;
        case 'r': case 'R':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w, y + h); glVertex2f(x + w, y + h/2);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y);
            glEnd();
            break;
        case 's': case 'S':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glVertex2f(x + w, y); glVertex2f(x, y);
            glEnd();
            break;
        case 't': case 'T':
            glBegin(GL_LINES);
            glVertex2f(x + w/2, y); glVertex2f(x + w/2, y + h);
            glVertex2f(x, y + h); glVertex2f(x + w, y + h);
            glEnd();
            break;
        case 'u': case 'U':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x, y);
            glVertex2f(x + w, y); glVertex2f(x + w, y + h);
            glEnd();
            break;
        case 'v': case 'V':
            glBegin(GL_LINES);
            glVertex2f(x, y + h); glVertex2f(x + w/2, y);
            glVertex2f(x + w/2, y); glVertex2f(x + w, y + h);
            glEnd();
            break;
        case 'w': case 'W':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x, y);
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w, y);
            glVertex2f(x + w, y + h);
            glEnd();
            break;
        case 'x': case 'X':
            glBegin(GL_LINES);
            glVertex2f(x, y); glVertex2f(x + w, y + h);
            glVertex2f(x + w, y); glVertex2f(x, y + h);
            glEnd();
            break;
        case 'y': case 'Y':
            glBegin(GL_LINES);
            glVertex2f(x, y + h); glVertex2f(x + w/2, y + h/2);
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w, y + h);
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w/2, y);
            glEnd();
            break;
        case 'z': case 'Z':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x + w, y + h);
            glVertex2f(x, y); glVertex2f(x + w, y);
            glEnd();
            break;
        default:
            // Простой прямоугольник для неизвестных букв
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y); glVertex2f(x + w, y); 
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glEnd();
    }
}

void TextRenderer::DrawSymbol(char symbol, float x, float y, float scale) {
    float w = 8 * scale;
    float h = 12 * scale;
    
    switch (symbol) {
        case '`': // Символ `
            glBegin(GL_LINES);
            glVertex2f(x + w*0.7f, y + h); glVertex2f(x + w*0.3f, y + h*0.7f);
            glEnd();
            break;
        case ' ': // Пробел - ничего не рисуем
            break;
        case ':':
            glPointSize(3.0f * scale);
            glBegin(GL_POINTS);
            glVertex2f(x + w/2, y + h/4);
            glVertex2f(x + w/2, y + 3*h/4);
            glEnd();
            break;
        case 'x':
            glBegin(GL_LINES);
            glVertex2f(x, y); glVertex2f(x + w, y + h);
            glVertex2f(x + w, y); glVertex2f(x, y + h);
            glEnd();
            break;
        case '/':
            glBegin(GL_LINES);
            glVertex2f(x + w, y); glVertex2f(x, y + h);
            glEnd();
            break;
        case '-':
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glEnd();
            break;
        case '.':
            glPointSize(3.0f * scale);
            glBegin(GL_POINTS);
            glVertex2f(x + w/2, y + h/8);
            glEnd();
            break;
        case '(':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y + h); glVertex2f(x + w/2, y + h/2);
            glVertex2f(x + w, y);
            glEnd();
            break;
        case ')':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x + w/2, y + h/2);
            glVertex2f(x, y);
            glEnd();
            break;
        default:
            // Простой прямоугольник для неизвестных символов
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y); glVertex2f(x + w, y); 
            glVertex2f(x + w, y + h); glVertex2f(x, y + h);
            glEnd();
    }
}

} // namespace Revolt