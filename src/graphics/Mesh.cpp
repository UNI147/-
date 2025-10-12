#include "Mesh.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

namespace Revolt {

Material::Material(float r, float g, float b, float a) {
    SetColor(r, g, b, a);
}

void Material::Apply() const {
    // ВКЛЮЧАЕМ цветовой материал для работы с glColor
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Устанавливаем цвет через glColor
    glColor4f(m_color[0], m_color[1], m_color[2], m_color[3]);
    
    // Дополнительные настройки материала для освещения
    float ambient[] = {m_color[0] * 0.5f, m_color[1] * 0.5f, m_color[2] * 0.5f, m_color[3]};
    float diffuse[] = {m_color[0], m_color[1], m_color[2], m_color[3]};
    float specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    float shininess = 16.0f;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    
    // Настройка прозрачности
    if (m_color[3] < 1.0f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    } else {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
}

void Material::Unapply() const {
    // Восстанавливаем стандартные настройки
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void Material::SetColor(float r, float g, float b, float a) {
    m_color[0] = r;
    m_color[1] = g;
    m_color[2] = b;
    m_color[3] = a;
}

Mesh::Mesh() : m_material(1.0f, 1.0f, 1.0f, 1.0f) {
}

Mesh::~Mesh() {
}

// PyramidMesh implementation
PyramidMesh::PyramidMesh(float base, float height) 
    : m_base(base), m_height(height) {
}

void PyramidMesh::Render() {
    m_material.Apply();
    
    float halfBase = m_base * 0.5f;
    
    glBegin(GL_TRIANGLES);
    
    // Устанавливаем цвет через glColor4f
    glColor4f(m_material.GetColor()[0], m_material.GetColor()[1], 
              m_material.GetColor()[2], m_material.GetColor()[3]);
    
    // ОСНОВАНИЕ - на плоскости y=-height/2 (центр пирамиды в середине высоты)
    float baseY = -m_height * 0.5f;
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-halfBase, baseY, -halfBase);
    glVertex3f( halfBase, baseY, -halfBase);
    glVertex3f( halfBase, baseY,  halfBase);
    
    glVertex3f( halfBase, baseY,  halfBase);
    glVertex3f(-halfBase, baseY,  halfBase);
    glVertex3f(-halfBase, baseY, -halfBase);
    
    // Боковые грани - основание на y=-height/2, вершина на y=height/2
    float topY = m_height * 0.5f;
    
    // Передняя грань
    glNormal3f(0.0f, 0.707f, -0.707f);
    glVertex3f(-halfBase, baseY, -halfBase);
    glVertex3f( 0.0f,     topY,  0.0f);  // вершина
    glVertex3f( halfBase, baseY, -halfBase);
    
    // Правая грань
    glNormal3f(0.707f, 0.707f, 0.0f);
    glVertex3f( halfBase, baseY, -halfBase);
    glVertex3f( 0.0f,     topY,  0.0f);  // вершина
    glVertex3f( halfBase, baseY,  halfBase);
    
    // Задняя грань
    glNormal3f(0.0f, 0.707f, 0.707f);
    glVertex3f( halfBase, baseY,  halfBase);
    glVertex3f( 0.0f,     topY,  0.0f);  // вершина
    glVertex3f(-halfBase, baseY,  halfBase);
    
    // Левая грань
    glNormal3f(-0.707f, 0.707f, 0.0f);
    glVertex3f(-halfBase, baseY,  halfBase);
    glVertex3f( 0.0f,     topY,  0.0f);  // вершина
    glVertex3f(-halfBase, baseY, -halfBase);
    
    glEnd();

    m_material.Unapply();
}

// CubeMesh implementation
CubeMesh::CubeMesh(float size) 
    : m_size(size) {
}

void CubeMesh::Render() {
    m_material.Apply();
    
    float halfSize = m_size * 0.5f;
    
    glBegin(GL_QUADS);
    
    // Устанавливаем цвет через glColor4f
    glColor4f(m_material.GetColor()[0], m_material.GetColor()[1], 
              m_material.GetColor()[2], m_material.GetColor()[3]);
    
    // Передняя грань
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    
    // Задняя грань
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize, -halfSize);
    
    // Верхняя грань
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    
    // Нижняя грань
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    
    // Правая грань
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    
    // Левая грань
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    
    glEnd();

    m_material.Unapply();
}

// TorusMesh implementation
TorusMesh::TorusMesh(float majorRadius, float minorRadius, int majorSegments, int minorSegments) 
    : m_majorRadius(majorRadius), m_minorRadius(minorRadius), 
      m_majorSegments(majorSegments), m_minorSegments(minorSegments) {
}

void TorusMesh::Render() {
    m_material.Apply();
    
    const float majorStep = 2.0f * 3.14159265359f / m_majorSegments;
    const float minorStep = 2.0f * 3.14159265359f / m_minorSegments;
    
    // Устанавливаем цвет материала
    glColor4f(m_material.GetColor()[0], m_material.GetColor()[1], 
              m_material.GetColor()[2], m_material.GetColor()[3]);
    
    // Уменьшаем базовый размер тора в 2 раза
    float scaleFactor = 0.5f;
    float scaledMajorRadius = m_majorRadius * scaleFactor;
    float scaledMinorRadius = m_minorRadius * scaleFactor;
    
    for (int i = 0; i < m_majorSegments; ++i) {
        glBegin(GL_QUAD_STRIP);
        
        float majorAngle1 = i * majorStep;
        float majorAngle2 = (i + 1) * majorStep;
        
        for (int j = 0; j <= m_minorSegments; ++j) {
            float minorAngle = j * minorStep;
            
            for (int k = 0; k < 2; ++k) {
                float currentMajorAngle = (k == 0) ? majorAngle1 : majorAngle2;
                
                // Вычисляем позицию точки на торе с уменьшенным масштабом
                float x = (scaledMajorRadius + scaledMinorRadius * cos(minorAngle)) * cos(currentMajorAngle);
                float y = (scaledMajorRadius + scaledMinorRadius * cos(minorAngle)) * sin(currentMajorAngle);
                float z = scaledMinorRadius * sin(minorAngle);
                
                // Вычисляем нормаль
                float nx = cos(currentMajorAngle) * cos(minorAngle);
                float ny = sin(currentMajorAngle) * cos(minorAngle);
                float nz = sin(minorAngle);
                
                glNormal3f(nx, ny, nz);
                glVertex3f(x, y, z);
            }
        }
        
        glEnd();
    }

    m_material.Unapply();
}

} // namespace Revolt