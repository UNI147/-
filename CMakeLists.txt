cmake_minimum_required(VERSION 3.10)
project(RotatingPyramid)

set(CMAKE_CXX_STANDARD 11)

# Настройка для Windows
if(WIN32)
    # Установите путь к GLFW если он не находится в стандартных местах
    set(GLFW_ROOT "C:/glfw-3.4.bin.WIN64" CACHE PATH "Path to GLFW")
    
    find_path(OPENGL_INCLUDE_DIR 
        NAMES GL/gl.h
        PATHS "C:/Program Files/Windows Kits/10/Include/10.0.22621.0/um/gl"
    )
    
    find_library(OPENGL_LIBRARY 
        NAMES opengl32 OpenGL32
        PATHS "C:/Program Files/Windows Kits/10/Lib/10.0.22621.0/um/x64"
    )
    
    find_path(GLFW_INCLUDE_DIR
        NAMES GLFW/glfw3.h
        PATHS ${GLFW_ROOT}/include
        /usr/include
    )
    
    find_library(GLFW_LIBRARY
        NAMES glfw3 glfw glfw3dll
        PATHS ${GLFW_ROOT}/lib-vc2022
              ${GLFW_ROOT}/lib
    )
    
    if(OPENGL_INCLUDE_DIR AND OPENGL_LIBRARY AND GLFW_INCLUDE_DIR AND GLFW_LIBRARY)
        message(STATUS "Found OpenGL: ${OPENGL_LIBRARY}")
        message(STATUS "Found GLFW: ${GLFW_LIBRARY}")
        
        add_executable(pyramid src/main.cpp)
        target_include_directories(pyramid PRIVATE ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR})
        target_link_libraries(pyramid ${OPENGL_LIBRARY} ${GLFW_LIBRARY})
    else()
        message(WARNING "Required libraries not found, trying fallback...")
        
        # Fallback: используем пакеты vcpkg если установлены
        find_package(glfw3 QUIET)
        find_package(OpenGL QUIET)
        
        if(glfw3_FOUND AND OpenGL_FOUND)
            add_executable(pyramid src/main.cpp)
            target_link_libraries(pyramid OpenGL::GL glfw)
        else()
            message(FATAL_ERROR "Required libraries (OpenGL or GLFW) not found! 
Please install GLFW from https://www.glfw.org/download.html and set GLFW_ROOT in CMakeLists.txt")
        endif()
    endif()
else()
    # Для Linux/Mac
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    add_executable(pyramid src/main.cpp)
    target_link_libraries(pyramid OpenGL::GL glfw)
endif()