#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "glm/glm.hpp"

template <class T = glm::u8vec4> struct Texture {
    std::size_t nx, ny;
    std::vector<T> img_data;

    Texture() = default;
    ~Texture() = default;

    explicit Texture(std::string const &path);

    T *data() { return img_data.data(); }

    std::size_t size() const
    {
        assert(nx * ny == img_data.size());
        return nx * ny;
    }
};
