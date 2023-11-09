#pragma once

#include "base/Buffer.h"
#include "base/Camera.h"
#include "base/Scene.h"
#include "glm/glm.hpp"

struct Manager {
    Scene m_scene;
    //Camera m_camera; // this should be virtual cameras

    Manager() = default;
    virtual ~Manager() = default;
};