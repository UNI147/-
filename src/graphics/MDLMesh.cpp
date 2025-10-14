#include "MDLMesh.h"
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>

namespace Revolt {

MDLMesh::MDLMesh(const std::string& filepath) 
    : m_currentFrame(0), m_hasAnimations(false), m_textureID(0) {
    LoadFromFile(filepath);
}

MDLMesh::~MDLMesh() {
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
    }
}

bool MDLMesh::LoadFromFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open MDL file: " << filepath << std::endl;
        return false;
    }
    
    // Упрощенная заглушка - создаем простую геометрию для теста
    std::cout << "MDL loader stub - creating test geometry for: " << filepath << std::endl;
    
    // Временная заглушка - создаем куб вместо реального парсера
    // TODO: Реализовать настоящий парсер .MDL
    
    m_hasAnimations = false;
    return true;
}

void MDLMesh::Render() {
    m_material.Apply();
    
    // Временная заглушка - рендерим куб вместо MDL модели
    glColor4f(m_material.GetColor()[0], m_material.GetColor()[1], 
              m_material.GetColor()[2], m_material.GetColor()[3]);
    
    float size = 0.5f;
    float halfSize = size * 0.5f;
    
    glBegin(GL_QUADS);
    
    // Передняя грань
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    
    // Задняя грань
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);
    
    // Верхняя грань
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    
    // Нижняя грань
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);
    
    // Правая грань
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    
    // Левая грань
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    
    glEnd();
    
    m_material.Unapply();
}

void MDLMesh::SetFrame(int frame) {
    if (frame >= 0 && frame < m_frames.size()) {
        m_currentFrame = frame;
    }
}

void MDLMesh::Animate(float deltaTime) {
    // Заглушка для анимации
}

} // namespace Revolt