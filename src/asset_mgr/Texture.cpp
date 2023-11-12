#include "asset_mgr/Texture.h"

#include <cstring>
#include <iostream>

#include "stb/stb_image.h"
#include <stdexcept>

Texture::Texture(std::string const &path)
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
