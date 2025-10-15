#pragma once
#include <memory>
#include <unordered_map>
#include <string>

namespace Revolt {
    class Mesh;
    class MDLModel; // Добавляем forward declaration
    
    class ResourceManager {
    public:
        static ResourceManager& GetInstance();
        
        // Загружает меш по имени типа ("Pyramid", "Cube", "Torus") с параметрами
        std::shared_ptr<Mesh> LoadMesh(const std::string& name, float param1 = 1.0f, float param2 = 1.0f, int param3 = 16, int param4 = 8);
        
        // Загружает MDL модель
        std::shared_ptr<MDLModel> LoadMDLModel(const std::string& filename);
        
    private:
        ResourceManager() = default;
        std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshCache;
        std::unordered_map<std::string, std::shared_ptr<MDLModel>> m_mdlCache; // Кэш для MDL моделей
    };
}