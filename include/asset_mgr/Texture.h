#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "glm/glm.hpp"

struct Texture {
    std::size_t m_nx, m_ny;
    std::vector<glm::u8vec3> img_data;

    Texture() = default;
    ~Texture() = default;

    explicit Texture(std::string const &path);

    glm::u8vec3 *data() { return img_data.data(); }

    std::size_t size() const
    {
        assert(m_nx * m_nx == img_data.size());
        return m_nx * m_nx;
    }

};
