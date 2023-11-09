// TODO
#pragma once

#include "glm/glm.hpp"

#include "asset_mgr/Texture.h"

template <class T = glm::u8vec4> struct fragment_shader_payload {
    glm::vec3 view_pos;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    Texture<T> *texture;

    fragment_shader_payload() { texture = nullptr; }

    explicit fragment_shader_payload(glm::vec3 const &col, glm::vec3 const &nor,
                            glm::vec2 const &tc, Texture<T> *tex)
        : color(col), normal(nor), tex_coords(tc), texture(tex)
    {
    }
};

struct vertex_shader_payload {
    glm::vec3 position;
};