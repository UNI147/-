cmake_minimum_required(VERSION 3.10)
project(RevoltEngine)

set(CMAKE_CXX_STANDARD 11)

# Исходные файлы движка
set(ENGINE_SOURCES
    src/main.cpp
    src/core/Application.cpp
    src/core/Window.cpp
    src/core/Scene.cpp
    src/core/SceneLoader.cpp
    src/core/ResourceManager.cpp
    src/core/GameObject.cpp
    src/graphics/TextRenderer.cpp
    src/graphics/Camera.cpp
    src/graphics/Mesh.cpp
    src/graphics/Renderer.cpp
    src/graphics/Framebuffer.cpp
    src/graphics/MDLMesh.cpp
    src/math/Matrix4.cpp
)

# Используем локальные библиотеки вместо FetchContent
option(USE_LOCAL_LIBS "Use local libraries" ON)

if(USE_LOCAL_LIBS)
    # GLFW
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw")
        add_subdirectory(libs/glfw)
        set(GLFW_LIBRARIES glfw)
    else()
        message(WARNING "Local GLFW not found, using FetchContent")
        include(FetchContent)
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.3.8
        )
        FetchContent_MakeAvailable(glfw)
        set(GLFW_LIBRARIES glfw)
    endif()
    
    # nlohmann/json (header-only)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/nlohmann")
        add_subdirectory(libs/nlohmann)
    else()
        find_package(nlohmann_json 3.11.2 QUIET)
        if(NOT nlohmann_json_FOUND)
            message(WARNING "Local nlohmann/json not found, using FetchContent")
            include(FetchContent)
            FetchContent_Declare(
                nlohmann_json
                GIT_REPOSITORY https://github.com/nlohmann/json.git
                GIT_TAG v3.11.2
            )
            FetchContent_MakeAvailable(nlohmann_json)
        endif()
    endif()
else()
    # Оригинальный код с FetchContent
    include(FetchContent)
    
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
    )
    FetchContent_MakeAvailable(glfw)
    
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# Настройка OpenGL
find_package(OpenGL REQUIRED)

# Создание исполняемого файла
add_executable(RevoltEngine ${ENGINE_SOURCES})

# Библиотеки для линковки
target_link_libraries(RevoltEngine 
    OpenGL::GL
    ${GLFW_LIBRARIES}
    nlohmann_json::nlohmann_json
)

# Настройка компилятора
if(MSVC)
    # Добавляем определения для компилятора MSVC
    target_compile_definitions(RevoltEngine PRIVATE _CRT_SECURE_NO_WARNINGS)
    # Отключаем некоторые предупреждения
    target_compile_options(RevoltEngine PRIVATE /W4)
endif()

# Для Windows
if(WIN32)
    target_link_libraries(RevoltEngine opengl32 gdi32)
    
endif()

# Установка выходной директории для исполняемых файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# Директории включения
target_include_directories(RevoltEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
)