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
    src/graphics/Camera.cpp
    src/graphics/Mesh.cpp
    src/graphics/Renderer.cpp
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

# Для Windows: настройки манифеста DPI
if(WIN32)
    target_link_libraries(RevoltEngine opengl32 gdi32)
    
    # ЯВНОЕ ОТКЛЮЧЕНИЕ DPI SCALING через компилятор
    if(MSVC)
        target_compile_definitions(RevoltEngine PRIVATE 
            _CRT_SECURE_NO_WARNINGS
            WIN32_LEAN_AND_MEAN
            NOMINMAX
        )
    endif()

    # ПРОСТОЙ СПОСОБ: используем существующий файл манифеста
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/manifest.xml")
        # Копируем существующий manifest.xml в бинарную директорию
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/manifest.xml"
            "${CMAKE_CURRENT_BINARY_DIR}/app.manifest"
            COPYONLY
        )
        
        # Добавляем манифест к целевому файлу
        set_target_properties(RevoltEngine PROPERTIES
            LINK_FLAGS "/MANIFEST:EMBED /MANIFESTINPUT:${CMAKE_CURRENT_BINARY_DIR}/app.manifest"
        )
    else()
        message(WARNING "manifest.xml not found, creating default manifest")
        
        # Создаем простой манифест без сложного экранирования
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/app.manifest" 
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
            "<assembly xmlns=\"urn:schemas-microsoft-com:asm.v1\" manifestVersion=\"1.0\">\n"
            "  <application xmlns=\"urn:schemas-microsoft-com:asm.v3\">\n"
            "    <windowsSettings>\n"
            "      <dpiAware xmlns=\"http://schemas.microsoft.com/SMI/2005/WindowsSettings\">false</dpiAware>\n"
            "      <dpiAwareness xmlns=\"http://schemas.microsoft.com/SMI/2016/WindowsSettings\">unaware</dpiAwareness>\n"
            "    </windowsSettings>\n"
            "  </application>\n"
            "  <compatibility xmlns=\"urn:schemas-microsoft-com:compatibility.v1\">\n"
            "    <application>\n"
            "      <supportedOS Id=\"{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}\"/>\n"
            "      <supportedOS Id=\"{1f676c76-80e1-4239-95bb-83d0f6d0da78}\"/>\n"
            "      <supportedOS Id=\"{4a2f28e3-53b9-4441-ba9c-d69d4a4a6e38}\"/>\n"
            "      <supportedOS Id=\"{35138b9a-5d96-4fbd-8e2d-a2440225f93a}\"/>\n"
            "    </application>\n"
            "  </compatibility>\n"
            "</assembly>\n"
        )
        
        set_target_properties(RevoltEngine PROPERTIES
            LINK_FLAGS "/MANIFEST:EMBED /MANIFESTINPUT:${CMAKE_CURRENT_BINARY_DIR}/app.manifest"
        )
    endif()
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