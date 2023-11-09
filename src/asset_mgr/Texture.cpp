#include "asset_mgr/Texture.h"

#include <cstring>
#include <iostream>

#include "stb/stb_image.h"
#include <stdexcept>

template <> Texture<glm::u8vec4>::Texture(std::string const &path)
{
    constexpr int dim = 4;
    int x{}, y{}, cmp = dim;
    unsigned char *p = stbi_load(path.c_str(), &x, &y, &cmp, cmp);
    if (cmp != dim) {
        throw std::invalid_argument("invalid component return");
    }
#ifndef NDEBUG
    std::cerr << "Texture width: " << nx << ", height: " << ny
              << ", channels: " << dim << ".\n";
#endif

    img_data.resize(x * y);
    std::memcpy(img_data.data(), p, img_data.size() * sizeof(img_data[0]));
    nx = static_cast<std::size_t>(x);
    ny = static_cast<std::size_t>(y);

    stbi_image_free(p);
}
