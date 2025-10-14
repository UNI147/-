#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>
#include "MDLMesh.h"

namespace Revolt {
    class Mesh;
    class ResourceManager {
    public:
        static ResourceManager& GetInstance();
        
        // Загружает меш по имени типа ("Pyramid", "Cube", "Torus") с параметрами
        std::shared_ptr<Mesh> LoadMesh(const std::string& name, float param1 = 1.0f, float param2 = 1.0f, int param3 = 16, int param4 = 8);
        std::shared_ptr<Mesh> LoadMDL(const std::string& filepath);

    private:
        ResourceManager() = default;
        std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshCache;
    };
}