#pragma once

#include "glm/glm.hpp"

// settings
constexpr int VERSION = 330;
constexpr int SCR_WIDTH = 1280;
constexpr int SCR_HEIGHT = 720;

enum class AA_Type : unsigned char {
    None = 0,
    MSAA, // TODO
    FXAA, // TODO
    SMAA, // TODO
};

enum class Shadow_Type : unsigned char {
    None = 0,
    Shadow_Mapping, // TODO
    PCSS,           // TODO
};

enum class AO_Type : unsigned char {
    None = 0,
    SSAO, // TODO
};

struct Config {
    AA_Type aa_type;
    Shadow_Type shadow_type;
    AO_Type ao_type;

    glm::u8vec4 clear_color;
    glm::u8vec4 light_color;
    glm::u8vec4 obj_color;

    Config():
        aa_type(AA_Type::None),
        shadow_type(Shadow_Type::None),
        ao_type(AO_Type::None),
        clear_color({221, 221, 221, 255}),
        light_color({255, 255, 255, 255}),
        obj_color({255, 255, 255, 255})
    {
    }
    virtual ~Config() = default;
};
