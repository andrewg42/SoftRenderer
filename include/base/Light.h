#pragma once

#include "glm/glm.hpp"

struct Light {
    glm::vec3 pos, color;
    //glm::vec3 dir;
    // float intensity;

    Light();
    ~Light();
};
