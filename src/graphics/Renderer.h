#pragma once
#include "Camera.h"
#include "Mesh.h"

namespace Revolt {

class Renderer {
public:
    Renderer();
    
    void Initialize(int width, int height);
    void BeginFrame();
    void EndFrame();
    void RenderMesh(Mesh& mesh, const Matrix4& transform);
    void SetCamera(const Camera& camera);
    
    void SetClearColor(float r, float g, float b, float a);

private:
    Camera m_camera;
};

} // namespace Revolt