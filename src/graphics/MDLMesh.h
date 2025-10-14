#pragma once
#include "Mesh.h"
#include <vector>
#include <string>

namespace Revolt {

class MDLMesh : public Mesh {
public:
    MDLMesh(const std::string& filepath);
    ~MDLMesh();
    
    bool LoadFromFile(const std::string& filepath);
    void Render() override;
    
    // Анимация
    void SetFrame(int frame);
    void Animate(float deltaTime);
    
private:
    struct MDLVertex {
        unsigned char x, y, z;
        unsigned char normal;
    };
    
    struct MDLTriangle {
        int facesfront;
        int vertices[3];
    };
    
    struct MDLFrame {
        std::string name;
        std::vector<MDLVertex> vertices;
    };
    
    std::vector<MDLFrame> m_frames;
    std::vector<MDLTriangle> m_triangles;
    std::vector<unsigned char> m_skinData;
    
    int m_currentFrame;
    int m_skinWidth, m_skinHeight;
    unsigned int m_textureID;
    bool m_hasAnimations;
    
    void CreateTexture();
    void CalculateNormals();
};

} // namespace Revolt