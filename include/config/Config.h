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

    glm::vec4 clear_color;
    glm::vec3 light_color;

    bool has_texture;
    glm::vec4 obj_color;

    bool enable_faceculling;
    bool enable_wireframe;

    Config():
            aa_type(AA_Type::None),
            shadow_type(Shadow_Type::None),
            ao_type(AO_Type::None),
            clear_color({221.0f / 255, 221.0f / 255, 221.0f / 255, 1.0f}),
            light_color({1.0f, 1.0f, 1.0f}),
            has_texture(false), obj_color({1.0f, 1.0f, 1.0f, 1.0f}),
            enable_faceculling(true), enable_wireframe(false)
    {
    }
    virtual ~Config() = default;
};
