#pragma once
#include <string>
#include <iostream>
#include "Scene.h"
#include "Camera.h"

namespace Revolt {

class SceneLoader {
public:
    static bool LoadSceneFromFile(const std::string& filepath, Scene& scene, Camera& camera);
    static void CreateDemoScene(Scene& scene, Camera& camera);
};

} // namespace Revolt