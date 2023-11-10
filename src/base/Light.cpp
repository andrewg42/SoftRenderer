#include "base/Light.h"

#include <iostream>

Light::Light() : pos({1.0f, 1.0f, 1.0f}), color({1.0f, 1.0f, 1.0f})
{
    //dir = -pos;
}

Light::~Light() = default;

void Light::update()
{
    ImGui_Context &gui = ImGui_Context::instance();
    color = gui.m_config.light_color;
}