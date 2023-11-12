#pragma once

#include "glm/glm.hpp"
#include "viewer/ImGui_Context.h"

struct Light {
    glm::vec3 pos, color; // point light source
    //glm::vec3 dir;
    // float intensity;

    Light();
    ~Light();

    void update();
};
