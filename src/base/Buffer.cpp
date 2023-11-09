#include "base/Buffer.h"

#include <cstring>
#include <iostream>
#include "config/Config.h"

Buffer_Type operator|(Buffer_Type a, Buffer_Type b)
{
    return static_cast<Buffer_Type>(static_cast<char>(a) |
                                    static_cast<char>(b));
}

bool operator&(Buffer_Type a, Buffer_Type b)
{
    return static_cast<bool>(static_cast<char>(a) & static_cast<char>(b));
}

Buffer::Buffer() : sample(1) {
    m_data.reserve(SCR_WIDTH * SCR_HEIGHT);
    m_color.reserve(SCR_WIDTH * SCR_HEIGHT * 4);
    m_depth.reserve(SCR_WIDTH * SCR_HEIGHT * 4);
}

Buffer::~Buffer() = default;

void Buffer::clear_color(glm::u8vec4 color)
{
    for(std::size_t i{}; i<m_color.size(); i++)
        m_color[i] = color;
}

void Buffer::clear_depth()
{
    for(std::size_t i{}; i<m_depth.size(); i++)
        m_depth[i] = 0.0f;
}

void Buffer::down_sampling(std::size_t width_out, std::size_t height_out)
{
    std::size_t width_in = sample * width_out;

    for (std::size_t y{}; y < height_out; y++) {
        for (std::size_t x{}; x < width_out; x++) {
            glm::u16vec4 sum(0, 0, 0, 0);

            for (std::size_t dy{}; dy < sample; dy++) {
                for (std::size_t dx{}; dx < sample; dx++) {
                    std::size_t idx = (y + dy) * width_in + x + dx;
                    sum += glm::u16vec4(m_color[idx]);
                }
            }

            glm::u8vec4 ret;
            ret.x = glm::u8(sum.x / sample / sample);
            ret.y = glm::u8(sum.y / sample / sample);
            ret.z = glm::u8(sum.z / sample / sample);
            ret.w = glm::u8(sum.w / sample / sample);

            std::size_t idx = y * width_out + x;
            m_data[idx] = ret;
        }
    }
}

void Buffer::resize(std::size_t const size)
{
    m_data.resize(size);
    m_color.resize(size * sample * sample);
    m_depth.resize(size * sample * sample);
}
