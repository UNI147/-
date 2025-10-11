#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

// Вершины пирамиды
float vertices[] = {
    // Основание
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    
    // Вершина
    0.0f, 0.5f, 0.0f
};

// Индексы
unsigned int indices[] = {
    0, 1, 2, 2, 3, 0, // Основание
    0, 1, 4,           // Грань 1
    1, 2, 4,           // Грань 2  
    2, 3, 4,           // Грань 3
    3, 0, 4            // Грань 4
};

// Функция для установки перспективной проекции
void setPerspective(float fov, float aspect, float nearPlane, float farPlane) {
    float f = 1.0f / tan(fov * 0.5f);
    float range = nearPlane - farPlane;
    
    float projection[16] = {0};
    projection[0] = f / aspect;
    projection[5] = f;
    projection[10] = (farPlane + nearPlane) / range;
    projection[11] = -1.0f;
    projection[14] = 2.0f * farPlane * nearPlane / range;
    
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
}

// Рисуем пирамиду
void drawPyramid() {
    glBegin(GL_TRIANGLES);
    
    for(int i = 0; i < 18; i += 3) {
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

int main() {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rotating Pyramid", nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Настройка OpenGL
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
    
    // Устанавливаем проекцию
    setPerspective(1.0472f, (float)WIDTH/HEIGHT, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Матрица модели-вида
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -3.0f); // Отодвигаем
        
        // Вращение
        glRotatef((float)glfwGetTime() * 50.0f, 0.0f, 1.0f, 0.0f);
        
        drawPyramid();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}