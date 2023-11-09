#pragma once

#include "utils/check_gl.h"
#include "glm/glm.hpp"
#include <memory>
#include <string>

#include "base/Buffer.h"
#include "config/Config.h"
#include "utils/Singleton.h"
#include "base/Input_Ctl.h"

struct Rasterizer : Singleton<Rasterizer> {
    Buffer m_buffer;
    Input_Ctl m_input;

    struct Private;
    std::unique_ptr<Private> p_private;

    Config m_config;

    Rasterizer();
    ~Rasterizer();

    void set_color(std::size_t, glm::u8vec4);
    void set_color(std::size_t, std::size_t, glm::u8vec4);
    void clear_color();
    void set_buffer_size(std::size_t const, std::size_t const);
    void add_model(std::string const &);

    char *data();

    void draw_line(glm::vec3 &, glm::vec3 &);
    void draw_triangle(glm::mat3 &, glm::vec3);
    void draw();
};
