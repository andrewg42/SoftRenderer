#pragma once

#include "glm/glm.hpp"
#include <optional>
#include <memory>

#include "asset_mgr/Texture.h"
#include "base/Light.h"

struct FragShader_Payload {
    glm::vec3 view_pos;
    glm::mat3 model_it;
    std::vector<Light> lights;
    glm::vec3 obj_color;
    glm::vec3 frag_pos;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    std::optional<std::shared_ptr<Texture<glm::u8vec3, glm::vec3>>> texture;

    FragShader_Payload() { texture = std::nullopt; }

    explicit FragShader_Payload(glm::vec3 const &pos, glm::vec3 const &nor, glm::vec2 const &tc,
        std::shared_ptr<Texture<glm::u8vec3, glm::vec3>> &pt)
        : frag_pos(pos), normal(nor), tex_coords(tc), texture(pt)
    {
    }
};

glm::vec3 blinn_phong_obj_color(FragShader_Payload &);
glm::vec3 blinn_phong_texture(FragShader_Payload &);
glm::vec3 show_normal(FragShader_Payload &);