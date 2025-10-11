import pygame
import math
import numpy as np

# Инициализация Pygame
pygame.init()

# Настройки окна
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("3D Rotating Cube")

# Цвета
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
BLUE = (0, 0, 255)

# Вершины куба (в 3D пространстве)
vertices = [
    [-1, -1, -1],
    [1, -1, -1],
    [1, 1, -1],
    [-1, 1, -1],
    [-1, -1, 1],
    [1, -1, 1],
    [1, 1, 1],
    [-1, 1, 1]
]

# Рёбра куба (соединения между вершинами)
edges = [
    (0, 1), (1, 2), (2, 3), (3, 0),
    (4, 5), (5, 6), (6, 7), (7, 4),
    (0, 4), (1, 5), (2, 6), (3, 7)
]

# Углы вращения
angle_x = angle_y = angle_z = 0

# Основной цикл
clock = pygame.time.Clock()
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Очистка экрана
    screen.fill(BLACK)

    # Вычисление матриц вращения
    rot_x = np.array([
        [1, 0, 0],
        [0, math.cos(angle_x), -math.sin(angle_x)],
        [0, math.sin(angle_x), math.cos(angle_x)]
    ])
    
    rot_y = np.array([
        [math.cos(angle_y), 0, math.sin(angle_y)],
        [0, 1, 0],
        [-math.sin(angle_y), 0, math.cos(angle_y)]
    ])
    
    rot_z = np.array([
        [math.cos(angle_z), -math.sin(angle_z), 0],
        [math.sin(angle_z), math.cos(angle_z), 0],
        [0, 0, 1]
    ])

    # Преобразование вершин
    transformed_vertices = []
    for vertex in vertices:
        # Применение вращений
        rotated = np.dot(rot_x, vertex)
        rotated = np.dot(rot_y, rotated)
        rotated = np.dot(rot_z, rotated)
        
        # Проекция 3D -> 2D
        scale = 200
        x = rotated[0] * scale + WIDTH // 2
        y = rotated[1] * scale + HEIGHT // 2
        transformed_vertices.append((x, y))

    # Отрисовка рёбер
    for edge in edges:
        pygame.draw.line(screen, BLUE, 
                         transformed_vertices[edge[0]], 
                         transformed_vertices[edge[1]], 2)

    # Обновление углов вращения
    angle_x += 0.01
    angle_y += 0.008
    angle_z += 0.005

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
