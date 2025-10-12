#include "Mesh.h"
#include <GLFW/glfw3.h>

namespace Revolt {

Mesh::Mesh() : m_indexCount(0) {
    // Конструктор - ничего не инициализируем для немедленного режима
}

Mesh::~Mesh() {
    // Деструктор - ничего не удаляем, так как используем немедленный режим
}

void Mesh::CreatePyramid() {
    m_indexCount = 18;
    // Просто устанавливаем количество индексов
    // В немедленном режиме не создаем буферы
}

void Mesh::Render() {
    // Вершины пирамиды
    float vertices[] = {
        // Основание
        -0.5f, -0.5f, -0.5f,  // 0
         0.5f, -0.5f, -0.5f,  // 1
         0.5f, -0.5f,  0.5f,  // 2
        -0.5f, -0.5f,  0.5f,  // 3
        
        // Вершина
        0.0f, 0.5f, 0.0f      // 4
    };

    // Индексы для треугольников
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // Основание
        0, 1, 4,           // Грань 1
        1, 2, 4,           // Грань 2  
        2, 3, 4,           // Грань 3
        3, 0, 4            // Грань 4
    };
    
    // Рендерим используя немедленный режим
    glBegin(GL_TRIANGLES);
    
    for(int i = 0; i < m_indexCount; i += 3) {
        int idx1 = indices[i];
        int idx2 = indices[i+1]; 
        int idx3 = indices[i+2];
        
        // Разные цвета для разных частей
        if(i < 6) {
            glColor3f(1.0f, 0.0f, 0.0f); // Основание - красное
        } else {
            // Боковые грани - разные цвета
            int face = (i - 6) / 3;
            switch(face) {
                case 0: glColor3f(0.0f, 1.0f, 0.0f); break; // зеленый
                case 1: glColor3f(0.0f, 0.0f, 1.0f); break; // синий  
                case 2: glColor3f(1.0f, 1.0f, 0.0f); break; // желтый
                case 3: glColor3f(1.0f, 0.0f, 1.0f); break; // пурпурный
            }
        }
        
        glVertex3f(vertices[idx1*3], vertices[idx1*3+1], vertices[idx1*3+2]);
        glVertex3f(vertices[idx2*3], vertices[idx2*3+1], vertices[idx2*3+2]);
        glVertex3f(vertices[idx3*3], vertices[idx3*3+1], vertices[idx3*3+2]);
    }
    
    glEnd();
}

} // namespace Revolt