#include "Framebuffer.h"
#include <iostream>
#include <vector>

namespace Revolt {

Framebuffer::Framebuffer(int width, int height) 
    : m_width(width), m_height(height), m_textureID(0) {
}

Framebuffer::~Framebuffer() {
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
    }
}

bool Framebuffer::Initialize() {
    // Создаем текстуру для низкого разрешения
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Выделяем память для текстуры (инициализируем нейтральным серым цветом)
    std::vector<unsigned char> textureData(m_width * m_height * 3, 0);
    for (size_t i = 0; i < textureData.size(); i += 3) {
        textureData[i] = 51;     // R: 0.2 * 255
        textureData[i + 1] = 51; // G: 0.2 * 255  (ИСПРАВЛЕНО: убрали синий оттенок)
        textureData[i + 2] = 51; // B: 0.2 * 255  (ИСПРАВЛЕНО: убрали синий оттенок)
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());
    
    // Устанавливаем параметры текстуры для пиксельного масштабирования
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    return true;
}

void Framebuffer::BeginRender() {
    // Устанавливаем вьюпорт для рендеринга в низком разрешении
    glViewport(0, 0, m_width, m_height);
    
    // Очищаем буферы НЕЙТРАЛЬНЫМ СЕРЫМ цветом
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // ИСПРАВЛЕНО: убрали синий оттенок
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::EndRender() {
    // Читаем пиксели из framebuffer'а
    std::vector<unsigned char> pixelData(m_width * m_height * 3);
    glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, pixelData.data());
    
    // Копируем в текстуру
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, pixelData.data());
}

void Framebuffer::RenderToScreen(int screenWidth, int screenHeight) {
    // Возвращаемся к полноэкранному вьюпорту
    glViewport(0, 0, screenWidth, screenHeight);
    
    // Очищаем экран черным
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Устанавливаем ортографическую проекцию для 2D рендеринга
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);  // Обратите внимание: Y инвертирован!
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Отключаем ненужные для 2D функции
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    // Включаем текстуру
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Вычисляем размеры для сохранения соотношения сторон
    float renderAspect = (float)m_width / (float)m_height;
    float screenAspect = (float)screenWidth / (float)screenHeight;
    
    int drawWidth, drawHeight;
    int drawX, drawY;
    
    if (screenAspect > renderAspect) {
        // Черные полосы по бокам
        drawHeight = screenHeight;
        drawWidth = (int)(screenHeight * renderAspect);
        drawX = (screenWidth - drawWidth) / 2;
        drawY = 0;
    } else {
        // Черные полосы сверху и снизу
        drawWidth = screenWidth;
        drawHeight = (int)(screenWidth / renderAspect);
        drawX = 0;
        drawY = (screenHeight - drawHeight) / 2;
    }
    
    // Рендерим текстуру с сохранением пропорций
    glColor3f(1.0f, 1.0f, 1.0f); // БЕЛЫЙ цвет для точного отображения текстуры
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f((float)drawX, (float)drawY);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((float)(drawX + drawWidth), (float)drawY);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((float)(drawX + drawWidth), (float)(drawY + drawHeight));
    glTexCoord2f(0.0f, 0.0f); glVertex2f((float)drawX, (float)(drawY + drawHeight));
    glEnd();
    
    // Восстанавливаем настройки
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

} // namespace Revolt