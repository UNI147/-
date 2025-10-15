#pragma once
#include "../math/Matrix4.h"

namespace Revolt {

struct Vertex {
    float x, y, z;
    float r, g, b, a;
    
    Vertex(float x, float y, float z, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
        : x(x), y(y), z(z), r(r), g(g), b(b), a(a) {}
};

class Material {
    public:
        Material(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
        void Apply() const;
        void Unapply() const;
        
        void SetColor(float r, float g, float b, float a = 1.0f);
        const float* GetColor() const { return m_color; }
        
        // Добавляем геттеры для отдельных компонентов
        float GetR() const { return m_color[0]; }
        float GetG() const { return m_color[1]; }
        float GetB() const { return m_color[2]; }
        float GetA() const { return m_color[3]; }
        
    private:
        float m_color[4];
    };

class Mesh {
public:
    Mesh();
    virtual ~Mesh();
    
    virtual void Render() = 0;
    void SetMaterial(const Material& material) { m_material = material; }
    const Material& GetMaterial() const { return m_material; }
    
protected:
    Material m_material;
};

// Конкретные реализации мешей
class PyramidMesh : public Mesh {
public:
    PyramidMesh(float base = 1.0f, float height = 1.0f);
    void Render() override;
    
private:
    void CreatePyramidGeometry(float base, float height);
    float m_base;
    float m_height;
};

class CubeMesh : public Mesh {
public:
    CubeMesh(float size = 1.0f);
    void Render() override;
    
private:
    void CreateCubeGeometry(float size);
    float m_size;
};

class TorusMesh : public Mesh {
public:
    TorusMesh(float majorRadius = 1.0f, float minorRadius = 0.3f, int majorSegments = 32, int minorSegments = 16);
    void Render() override;
    
private:
    void CreateTorusGeometry(float majorRadius, float minorRadius, int majorSegments, int minorSegments);
    float m_majorRadius;
    float m_minorRadius;
    int m_majorSegments;
    int m_minorSegments;
};

} // namespace Revolt