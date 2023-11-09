#pragma once

#include <iostream>

#include "glm/glm.hpp"

#include "utils/Noncopyable.h"

struct Camera : Noncopyable {
    glm::vec3 eye, look_at, up_vec, keep_up_axis;
    glm::mat4 lookAt_mat;
    bool changed;

    Camera();
    ~Camera();

    glm::mat4 get_proj_mat_persp(float, float, float, float);
    glm::mat4 get_lookAt_mat();
};
