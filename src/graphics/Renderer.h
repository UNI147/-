#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Framebuffer.h"
#include "MDLModel.h"

namespace Revolt {

class Renderer {
public:
    Renderer();
    
    void Initialize(int width, int height);
    void BeginFrame();
    void EndFrame();
    void RenderMesh(Mesh& mesh, const Matrix4& transform);
    void SetCamera(const Camera& camera);
    void RenderToScreen(int screenWidth, int screenHeight);
    void SetClearColor(float r, float g, float b, float a);
    void RenderMDLModel(MDLModel& model, const Matrix4& transform, int frame = 0);

private:
    Camera m_camera;
    Framebuffer m_framebuffer; // Новый член
};

} // namespace Revolt