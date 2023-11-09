#pragma once

#include <vector>
#include <string>
#include "glm/glm.hpp"

#include "asset_mgr/Material.h"

struct Mesh {
    std::string mesh_name;
    std::vector<glm::vec3> pos, norm;
    std::vector<glm::vec2> uv;
    std::vector<glm::uvec3> face;
    //std::vector<std::size_t> f_pos, f_norm, f_uv;
    Material mesh_material;
};
