#include "ResourceManager.h"
#include "../graphics/Mesh.h"
#include <iostream>

namespace Revolt {
    ResourceManager& ResourceManager::GetInstance() {
        static ResourceManager instance;
        return instance;
    }
    
    std::shared_ptr<Mesh> ResourceManager::LoadMesh(const std::string& name, float param1, float param2, int param3, int param4) {
        std::string key = name + "_" + std::to_string(param1) + "_" + std::to_string(param2) + "_" + std::to_string(param3) + "_" + std::to_string(param4);
        
        auto it = m_meshCache.find(key);
        if (it != m_meshCache.end()) {
            std::cout << "Using cached mesh: " << key << std::endl;
            return it->second;
        }
        
        std::shared_ptr<Mesh> mesh;
        if (name == "Pyramid") {
            mesh = std::make_shared<PyramidMesh>(param1, param2);
        } else if (name == "Cube") {
            mesh = std::make_shared<CubeMesh>(param1);
        } else if (name == "Torus") {
            mesh = std::make_shared<TorusMesh>(param1, param2, param3, param4);
        }
        
        if (mesh) {
            std::cout << "Created new mesh: " << key << std::endl;
            m_meshCache[key] = mesh;
        }
        
        return mesh;
    }

    std::shared_ptr<Mesh> ResourceManager::LoadMDL(const std::string& filepath) {
        std::string key = "MDL_" + filepath;
        
        auto it = m_meshCache.find(key);
        if (it != m_meshCache.end()) {
            std::cout << "Using cached MDL: " << key << std::endl;
            return it->second;
        }
        
        auto mesh = std::make_shared<MDLMesh>(filepath);
        if (mesh) {
            std::cout << "Created new MDL mesh: " << key << std::endl;
            m_meshCache[key] = mesh;
        }
        
        return mesh;
    }
}