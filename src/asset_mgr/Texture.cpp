#include "asset_mgr/Texture.h"

#include <cstring>
#include <iostream>

#include "stb/stb_image.h"
#include <stdexcept>

// load textures
template <>
Texture<glm::u8vec3, glm::vec3>::Texture(std::string const &path)
{
    constexpr int dim = 3;
    int nx{}, ny{}, cmp = dim;
    unsigned char *p = stbi_load(path.c_str(), &nx, &ny, &cmp, cmp);
    if (cmp != dim) {
        std::cerr << "cmp = " << cmp << '\n';
        throw std::invalid_argument("invalid component return");
    }
#ifndef NDEBUG
    std::cerr << "Texture width: " << nx << ", height: " << ny
              << ", channels: " << dim << ".\n";
#endif

    img_data.resize(nx * ny);
    std::memcpy(img_data.data(), p, img_data.size() * sizeof(img_data[0]));
    m_nx = static_cast<std::size_t>(nx);
    m_ny = static_cast<std::size_t>(ny);
}

template <>
Texture<glm::u8vec4, glm::vec4>::Texture(std::string const &path)
{
    constexpr int dim = 4;
    int nx{}, ny{}, cmp = dim;
    unsigned char *p = stbi_load(path.c_str(), &nx, &ny, &cmp, cmp);
    if (cmp != dim) {
        std::cerr << "cmp = " << cmp << '\n';
        throw std::invalid_argument("invalid component return");
    }
#ifndef NDEBUG
    std::cerr << "Texture width: " << nx << ", height: " << ny
              << ", channels: " << dim << ".\n";
#endif

    img_data.resize(nx * ny);
    std::memcpy(img_data.data(), p, img_data.size() * sizeof(img_data[0]));
    m_nx = static_cast<std::size_t>(nx);
    m_ny = static_cast<std::size_t>(ny);
}

static std::size_t get_idx(std::size_t nx, std::size_t x, std::size_t y) {
    return y * nx + x;
}

static glm::vec3 u82float(glm::u8vec3 c) {
    return glm::vec3(static_cast<float>(c.x) / 255.0f,
                    static_cast<float>(c.y) / 255.0f,
                    static_cast<float>(c.z) / 255.0f);
}

// tex2D on texture
template <> glm::vec3 Texture<glm::u8vec3, glm::vec3>::tex2D(float u, float v)
{
    float x = u * m_nx, y = (1-v) * m_ny;
    std::size_t x_floor = glm::floor(x), x_ceil = glm::ceil(x);
    std::size_t y_floor = glm::floor(y), y_ceil = glm::ceil(y);

    // bilinear interpolation
    auto color_bottomleft =
        u82float(img_data.at(get_idx(m_nx, x_floor, y_floor)));
    auto color_bottomright =
        u82float(img_data.at(get_idx(m_nx, x_ceil, y_floor)));
    auto color_topleft = u82float(img_data.at(get_idx(m_nx, x_floor, y_ceil)));
    auto color_topright = u82float(img_data.at(get_idx(m_nx, x_ceil, y_ceil)));
    
    auto color_bottom =
        color_bottomleft * (x - x_floor) + color_bottomright * (x_ceil - x);
    auto color_top =
        color_topleft * (x - x_floor) + color_topright * (x_ceil - x);

    auto color =
        color_bottom * (y - y_floor) + color_top * (y_ceil - y);
    
    return color;
}

// tex2D on shadow map
template <> float Texture<float, float>::tex2D(float u, float v) {
    float x = u * m_nx, y = (1 - v) * m_ny;
    std::size_t x_floor = glm::floor(x), x_ceil = glm::ceil(x);
    std::size_t y_floor = glm::floor(y), y_ceil = glm::ceil(y);

    // bilinear interpolation
    auto color_bottomleft =
        img_data.at(get_idx(m_nx, x_floor, y_floor));
    auto color_bottomright =
        img_data.at(get_idx(m_nx, x_ceil, y_floor));
    auto color_topleft = img_data.at(get_idx(m_nx, x_floor, y_ceil));
    auto color_topright = img_data.at(get_idx(m_nx, x_ceil, y_ceil));

    auto color_bottom =
        color_bottomleft * (x - x_floor) + color_bottomright * (x_ceil - x);
    auto color_top =
        color_topleft * (x - x_floor) + color_topright * (x_ceil - x);

    auto color = color_bottom * (y - y_floor) + color_top * (y_ceil - y);

    return color;
}