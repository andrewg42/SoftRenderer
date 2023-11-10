#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "utils/Singleton.h"

enum class Buffer_Type : unsigned char {
    color = 1,
    depth = 2,
};

Buffer_Type operator|(Buffer_Type, Buffer_Type); // set
bool operator&(Buffer_Type, Buffer_Type);        // get

struct Buffer : Singleton<Buffer> {
    std::vector<glm::u8vec4> m_data;
    std::vector<glm::u8vec4> m_color;
    std::vector<float> m_depth;
    int sample;

    Buffer();
    ~Buffer();

    void clear_color(glm::u8vec4 color = {221, 221, 221, 255});
    void clear_depth();
    void down_sampling(std::size_t, std::size_t);

    void resize(std::size_t const);
    void clear(Buffer_Type op);
};
