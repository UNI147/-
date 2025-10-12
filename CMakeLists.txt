cmake_minimum_required(VERSION 3.10)
project(RevoltEngine)

set(CMAKE_CXX_STANDARD 11)

# Исходные файлы движка
set(ENGINE_SOURCES
    src/main.cpp
    src/core/Application.cpp
    src/core/Window.cpp
    src/graphics/Camera.cpp
    src/graphics/Mesh.cpp
    src/graphics/Renderer.cpp
    src/math/Matrix4.cpp
)

# Настройка GLFW
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)

FetchContent_MakeAvailable(glfw)

# Настройка OpenGL
find_package(OpenGL REQUIRED)

# Создание исполняемого файла
add_executable(RevoltEngine ${ENGINE_SOURCES})

# Директории включения
target_include_directories(RevoltEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLFW_SOURCE_DIR}/include
)

# Библиотеки для линковки
target_link_libraries(RevoltEngine 
    OpenGL::GL
    glfw
)

# Для Windows также может потребоваться линковка с системными библиотеками
if(WIN32)
    target_link_libraries(RevoltEngine opengl32 gdi32)
endif()

# Установка выходной директории для исполняемых файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)