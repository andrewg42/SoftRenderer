#pragma once

#include "base/Buffer.h"
#include "base/Camera.h"
#include "base/Scene.h"
#include "glm/glm.hpp"
#include "asset_mgr/Texture.h"

struct Manager {
    Scene m_scene;
    //Camera m_camera; // this should be virtual cameras

    std::vector<std::shared_ptr<Texture>> vec_texture;
    Manager() = default;
    virtual ~Manager() = default;

    void update();
};