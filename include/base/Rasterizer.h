#pragma once

#include "utils/check_gl.h"
#include "glm/glm.hpp"
#include <memory>
#include <string>
#include <functional>

#include "base/Buffer.h"
#include "config/Config.h"
#include "utils/Singleton.h"
#include "base/Input_Ctl.h"
#include "base/IShader.h"

struct Rasterizer : Singleton<Rasterizer> {
    Buffer m_buffer;
    Input_Ctl m_input;

    struct Private;
    std::unique_ptr<Private> p_private;

    Config m_config;

    std::function<glm::vec3(FragShader_Payload &)> frag_shader;
    FragShader_Payload fsp;

    Rasterizer();
    ~Rasterizer();

    void draw();
    char *data();
    void add_model(std::string const &);
    void add_texture(std::string const &);
    void set_buffer_size(std::size_t const, std::size_t const);

private:
    void set_color(std::size_t, glm::u8vec4);
    void set_color(std::size_t, glm::vec4);
    void set_color(std::size_t, std::size_t, glm::u8vec4);

    void tick();
    void draw_line(glm::vec3 &, glm::vec3 &);
};
