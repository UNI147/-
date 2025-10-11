#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

// Вершины пирамиды (основание + вершина)
float vertices[] = {
    // Основание (квадрат)
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    
    // Вершина
    0.0f, 0.5f, 0.0f
};

// Индексы для построения треугольников
unsigned int indices[] = {
    // Основание
    0, 1, 2,
    2, 3, 0,
    
    // Боковые грани
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

// Простейшая матрица проекции
void perspective(float fov, float aspect, float near, float far, float* mat) {
    float f = 1.0f / tan(fov * 0.5f);
    float range = near - far;
    
    mat[0] = f / aspect; mat[4] = 0;   mat[8]  = 0;                     mat[12] = 0;
    mat[1] = 0;          mat[5] = f;   mat[9]  = 0;                     mat[13] = 0;
    mat[2] = 0;          mat[6] = 0;   mat[10] = (far + near) / range;  mat[14] = 2 * far * near / range;
    mat[3] = 0;          mat[7] = 0;   mat[11] = -1;                    mat[15] = 0;
}

// Умножение матриц 4x4
void multiplyMatrices(float* a, float* b, float* result) {
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result[i*4 + j] = 0;
            for(int k = 0; k < 4; ++k) {
                result[i*4 + j] += a[i*4 + k] * b[k*4 + j];
            }
        }
    }
}

// Создание матрицы поворота
void rotationMatrix(float angle, float x, float y, float z, float* mat) {
    float c = cos(angle);
    float s = sin(angle);
    float t = 1 - c;
    
    mat[0] = t*x*x + c;   mat[4] = t*x*y - s*z; mat[8]  = t*x*z + s*y;  mat[12] = 0;
    mat[1] = t*x*y + s*z; mat[5] = t*y*y + c;   mat[9]  = t*y*z - s*x;  mat[13] = 0;
    mat[2] = t*x*z - s*y; mat[6] = t*y*z + s*x; mat[10] = t*z*z + c;    mat[14] = 0;
    mat[3] = 0;           mat[7] = 0;           mat[11] = 0;            mat[15] = 1;
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

    float projection[16];
    perspective(1.0472f, (float)WIDTH/HEIGHT, 0.1f, 100.0f, projection);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Матрица модели-вида
        float modelview[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,-3,
            0,0,0,1
        };
        
        // Добавляем вращение
        float rotation[16];
        rotationMatrix((float)glfwGetTime(), 0, 1, 0, rotation);
        
        float mvp[16];
        multiplyMatrices(projection, modelview, mvp);
        multiplyMatrices(mvp, rotation, mvp);
        
        // Устанавливаем матрицу преобразования
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(mvp);
        
        // Рисуем пирамиду
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Красный
        
        for(int i = 0; i < 18; i += 3) {
            int idx1 = indices[i] * 3;
            int idx2 = indices[i+1] * 3;
            int idx3 = indices[i+2] * 3;
            
            // Чередуем цвета для граней
            if(i >= 6) glColor3f(0.0f, (i%3)*0.3f + 0.4f, 0.7f);
            
            glVertex3f(vertices[idx1], vertices[idx1+1], vertices[idx1+2]);
            glVertex3f(vertices[idx2], vertices[idx2+1], vertices[idx2+2]);
            glVertex3f(vertices[idx3], vertices[idx3+1], vertices[idx3+2]);
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}