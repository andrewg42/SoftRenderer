#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <iostream>

#include "utils/Singleton.h"
#include "config/Config.h"

struct ImGui_Context : Singleton<ImGui_Context> {
    Config m_config;

    ImGui_Context()
    {}
    ~ImGui_Context() = default;


    void init_imgui(GLFWwindow *window);
	void start_frame();
    void draw_panel();
    void end_frame();
};