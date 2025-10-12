#pragma once

namespace Revolt {

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    void CreatePyramid();
    void Render();
    
private:
    int m_indexCount;
};

} // namespace Revolt