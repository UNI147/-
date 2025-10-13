#include "TextRenderer.h"
#include <iostream>

namespace Revolt {

TextRenderer::TextRenderer() 
    : m_windowWidth(800), m_windowHeight(600), m_initialized(false), 
      m_renderWidth(800), m_renderHeight(600) {
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

void TextRenderer::SetRenderResolution(int width, int height) {
    m_renderWidth = width;
    m_renderHeight = height;
}

void TextRenderer::RenderText(const std::string& text, float x, float y, float scale) {
    if (!m_initialized) return;
    
    // Сохраняем текущие настройки OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    
    // Устанавливаем 2D проекцию (координаты: 0,0 в левом ВЕРХНЕМ углу)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_windowWidth, m_windowHeight, 0, -1, 1); // 0,0 в верхнем левом углу

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
    float bgHeight = 20 * scale;

    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(x - 8, y); // Верхний левый
    glVertex2f(x + bgWidth, y); // Верхний правый
    glVertex2f(x + bgWidth, y + bgHeight); // Нижний правый
    glVertex2f(x - 8, y + bgHeight); // Нижний левый
    glEnd();

    // Цвет текста - БЕЛЫЙ, тонкие линии
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.4f);
    
    float currentX = x;
    
    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        
        if (c == ' ') {
            currentX += 8 * scale;
            continue;
        }
        
        // Рисуем символы (переворачиваем Y-координаты)
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
    
    // ИСПРАВЛЕНИЕ: переворачиваем Y-координаты
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
            glVertex2f(x, y); glVertex2f(x + w, y); // ВЕРХ
            glVertex2f(x + w, y + h/2); glVertex2f(x, y + h/2);
            glVertex2f(x, y + h); glVertex2f(x + w, y + h); // НИЗ
            glEnd();
            break;
        case '3':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x + w, y); // ВЕРХ
            glVertex2f(x + w, y + h); glVertex2f(x, y + h); // НИЗ
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w*0.7f, y + h/2);
            glEnd();
            break;
        case '4':
            glBegin(GL_LINES);
            glVertex2f(x, y); glVertex2f(x, y + h/2); // Левая вертикаль
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2); // Горизонталь
            glVertex2f(x + w, y); glVertex2f(x + w, y + h); // Правая вертикаль
            glEnd();
            break;
        case '5':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y); glVertex2f(x, y); // ВЕРХ
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h); // НИЗ
            glEnd();
            break;
        case '6':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y); glVertex2f(x, y); // ВЕРХ
            glVertex2f(x, y + h); glVertex2f(x + w, y + h); // НИЗ
            glVertex2f(x + w, y + h/2); glVertex2f(x, y + h/2);
            glEnd();
            break;
        case '7':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x + w, y); // ВЕРХ
            glVertex2f(x + w, y + h); // НИЗ
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
            glVertex2f(x, y + h); glVertex2f(x, y); // Левая сторона
            glVertex2f(x + w, y); glVertex2f(x + w, y + h); // Правая сторона
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2); // Средняя линия
            glEnd();
            break;
    }
}

void TextRenderer::DrawLetter(char letter, float x, float y, float scale) {
    float w = 8 * scale;
    float h = 12 * scale;
    
    // ИСПРАВЛЕНИЕ: переворачиваем Y-координаты
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
            glVertex2f(x, y + h); // Начинаем снизу слева
            glVertex2f(x, y); // Поднимаемся вверх
            glVertex2f(x + w*0.7f, y); // Верхняя горизонталь
            glVertex2f(x + w, y + h*0.3f); // Верхний правый изгиб
            glVertex2f(x + w*0.7f, y + h/2); // Средняя горизонталь справа
            glVertex2f(x + w, y + h*0.7f); // Нижний правый изгиб
            glVertex2f(x + w*0.7f, y + h); // Нижняя горизонталь
            glVertex2f(x, y + h); // Замыкаем в начальной точке
            glEnd();
            break;
        case 'c': case 'C':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y); glVertex2f(x, y); // ВЕРХ
            glVertex2f(x, y + h); glVertex2f(x + w, y + h); // НИЗ
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
            glVertex2f(x + w, y); glVertex2f(x, y); // ВЕРХ
            glVertex2f(x, y + h); glVertex2f(x + w, y + h); // НИЗ
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w*0.7f, y + h/2);
            glEnd();
            break;
        case 'f': case 'F':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w*0.7f, y); // Верхняя точка правой части
            glVertex2f(x, y); // Верхний левый угол
            glVertex2f(x, y + h); // Левая вертикаль вниз
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w*0.7f, y + h/2); // Средняя горизонталь
            glEnd();
            break;
        case 'g': case 'G':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y); glVertex2f(x, y); // ВЕРХ
            glVertex2f(x, y + h); glVertex2f(x + w, y + h); // НИЗ
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
            glVertex2f(x + w, y); glVertex2f(x + w/2, y); // ВЕРХ
            glVertex2f(x + w/2, y + h); // НИЗ
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
            glVertex2f(x, y); glVertex2f(x, y + h);
            glVertex2f(x + w, y + h);
            glEnd();
            break;
        case 'm': case 'M':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x, y); // Левая сторона
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w, y); // Правая сторона
            glVertex2f(x + w, y + h);
            glEnd();
            break;
        case 'n': case 'N':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y + h); glVertex2f(x, y); // Левая сторона
            glVertex2f(x + w, y + h); glVertex2f(x + w, y); // Правая сторона
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
            glVertex2f(x, y + h); glVertex2f(x, y); // Левая сторона
            glVertex2f(x + w, y); glVertex2f(x + w, y + h/2); // Верхняя часть
            glVertex2f(x, y + h/2); // Средняя линия
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
            glVertex2f(x, y + h); glVertex2f(x, y); // Левая сторона
            glVertex2f(x + w, y); glVertex2f(x + w, y + h/2); // Верхняя часть
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h); // Диагональ
            glEnd();
            break;
        case 's': case 'S':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y); glVertex2f(x, y); // ВЕРХ
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glVertex2f(x + w, y + h); glVertex2f(x, y + h); // НИЗ
            glEnd();
            break;
        case 't': case 'T':
            glBegin(GL_LINES);
            glVertex2f(x + w/2, y); glVertex2f(x + w/2, y + h);
            glVertex2f(x, y); glVertex2f(x + w, y); // ВЕРХ
            glEnd();
            break;
        case 'u': case 'U':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h); // Левая сторона
            glVertex2f(x + w, y + h); glVertex2f(x + w, y); // Правая сторона
            glEnd();
            break;
        case 'v': case 'V':
            glBegin(GL_LINES);
            glVertex2f(x, y); glVertex2f(x + w/2, y + h); // Левая диагональ
            glVertex2f(x + w/2, y + h); glVertex2f(x + w, y); // Правая диагональ
            glEnd();
            break;
        case 'w': case 'W':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x, y + h); // Левая сторона
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w, y + h); // Средняя часть
            glVertex2f(x + w, y); // Правая сторона
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
            glVertex2f(x, y); glVertex2f(x + w/2, y + h/2); // Левая ветвь
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w, y); // Правая ветвь
            glVertex2f(x + w/2, y + h/2); glVertex2f(x + w/2, y + h); // Ствол
            glEnd();
            break;
        case 'z': case 'Z':
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); glVertex2f(x + w, y); // ВЕРХ
            glVertex2f(x, y + h); glVertex2f(x + w, y + h); // НИЗ
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
            glVertex2f(x + w*0.7f, y); glVertex2f(x + w*0.3f, y + h*0.3f);
            glEnd();
            break;
        case ' ': // Пробел - ничего не рисуем
            break;
        case ':': // ИСПРАВЛЕННОЕ двоеточие
            glEnable(GL_POINT_SMOOTH);
            glPointSize(3.0f * scale);
            glBegin(GL_POINTS);
            glVertex2f(x + w/2, y + h/4);
            glVertex2f(x + w/2, y + 3*h/4);
            glEnd();
            glDisable(GL_POINT_SMOOTH);
            break;
        case '/': // ИСПРАВЛЕННЫЙ слэш
            glBegin(GL_LINES);
            glVertex2f(x + w, y); glVertex2f(x, y + h);
            glEnd();
            break;
        case '-': // Дефис
            glBegin(GL_LINES);
            glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2);
            glEnd();
            break;
        case '.': // Точка
            glEnable(GL_POINT_SMOOTH);
            glPointSize(3.0f * scale);
            glBegin(GL_POINTS);
            glVertex2f(x + w/2, y + h/8);
            glEnd();
            glDisable(GL_POINT_SMOOTH);
            break;
        case '(': // Левая скобка
            glBegin(GL_LINE_STRIP);
            glVertex2f(x + w, y); 
            glVertex2f(x + w/2, y + h/2);
            glVertex2f(x + w, y + h);
            glEnd();
            break;
        case ')': // Правая скобка
            glBegin(GL_LINE_STRIP);
            glVertex2f(x, y); 
            glVertex2f(x + w/2, y + h/2);
            glVertex2f(x, y + h);
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