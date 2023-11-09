#pragma once

#include "config/Config.h"
#include "glm/glm.hpp"

#include "utils/Singleton.h"
#include "base/Camera.h"
#include "config/Config.h"

struct Input_Ctl: Singleton<Input_Ctl> {
    int m_width, m_height;
    int scr_width, scr_height; // for mouse pos callback
    Camera m_camera;
    float fovy, zNear, zFar;
    float translate_speed;

    Input_Ctl() : fovy(40.0f), zNear(0.1f), zFar(100.0f), m_width(SCR_WIDTH),
          m_height(SCR_HEIGHT), scr_width(SCR_WIDTH), scr_height(SCR_HEIGHT)
    {
        translate_speed = 28.0f; // TODO
    }
    ~Input_Ctl() = default;

    glm::mat4 get_proj();

    void rotate(float, float, float, float);
    void translate(float, float, float, float);
    void zoom(float);
};