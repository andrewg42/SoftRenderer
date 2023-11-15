#include "base/IShader.h"

#include <iostream>
#include "math/Mat_Op.h"

constexpr int NUM_SAMPLES = 20;
constexpr int NUM_RINGS = 10;
constexpr float PI = 3.141592653589793f;
constexpr float PI2 = 6.283185307179586f;
constexpr float EPS = 1e-3;
glm::vec2 poissonDisk[NUM_SAMPLES];

static glm::vec3 clamp(glm::vec3 color) {
    return glm::vec3(
                (color.x > 1.0f) ? 1.0f : color.x,
                (color.y > 1.0f) ? 1.0f : color.y,
                (color.z > 1.0f) ? 1.0f : color.z
           );
}

static float rand_1to1(float x)
{
    // -1 -1
    return glm::fract(glm::sin(x) * 10000.0f);
}

static float rand_2to1(glm::vec2 uv)
{
    // 0 - 1
    const float a = 12.9898f, b = 78.233f, c = 43758.5453f;
    float dt = glm::dot(uv, glm::vec2(a, b)), sn = glm::mod(dt, PI);
    return glm::fract(sin(sn) * c);
}

static void poissonDiskSamples(const glm::vec2 randomSeed)
{
    float ANGLE_STEP = PI2 * float(NUM_RINGS) / float(NUM_SAMPLES);
    float INV_NUM_SAMPLES = 1.0f / float(NUM_SAMPLES);

    float angle = rand_2to1(randomSeed) * PI2;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        poissonDisk[i] = glm::vec2(glm::cos(angle), glm::sin(angle)) *
                         glm::pow(radius, 0.75f);
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}
static float findBlocker(Texture<float, float> shadowMap, glm::vec2 uv, float zReceiver)
{
    float tot_depth = 0.0;
    int cnt = 0;
    float filter_size = 1.0 / 1024.0;

    poissonDiskSamples(uv);
    for (int i = 0; i < NUM_SAMPLES; i++) {
        glm::vec2 simple_uv = uv + poissonDisk[i] * filter_size;
        float sm_depth = shadowMap.tex2D(simple_uv.x, simple_uv.y);
        if (zReceiver > sm_depth + EPS) {
            tot_depth += sm_depth;
            cnt++;
        }
    }
    // not blocked
    if (cnt == 0)
        return -1.0;
    // all blocked
    if (cnt == NUM_SAMPLES)
        return 2.0;

    return tot_depth / float(cnt);
}

float PCSS(Texture<float, float> shadowMap, glm::vec4 coords)
{
    float bias = EPS;
    float cur_depth = coords.z;

    // STEP 1: avgblocker depth
    float blocker_avg_depth = findBlocker(shadowMap, glm::vec2(coords.x, coords.y), cur_depth);
    // not blocked
    if (blocker_avg_depth < -EPS)
        return 1.0;
    // all blocked
    if (blocker_avg_depth > 1.0 + EPS)
        return 0.0;

    // STEP 2: penumbra size
    float penumbra = (cur_depth - blocker_avg_depth) / blocker_avg_depth;

    // STEP 3: filtering
    float visibility = 0.0;
    float coef = 1.0 / 2048.0 * 5.0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        glm::vec2 sm_coords = poissonDisk[i] * coef * penumbra + glm::vec2(coords.x, coords.y);
        float sm_depth = shadowMap.tex2D(sm_coords.x, sm_coords.y);
        visibility += (cur_depth > sm_depth + bias) ? 0.0 : 1.0;
    }

    return visibility / float(NUM_SAMPLES);
}


glm::vec3 phong_obj_color(FragShader_Payload &fsp)
{
    glm::vec3 FragColor = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 norm = glm::normalize(fsp.model_it * fsp.normal);
    for (auto light : fsp.lights) {
        // ambient
        float ambientStrength = 0.1f;
        glm::vec3 ambient = ambientStrength * light.color;

        // diffuse
        glm::vec3 light_dir = glm::normalize(light.pos - fsp.frag_pos);
        float diff = glm::max(glm::dot(norm, light_dir), 0.0f);
        glm::vec3 diffuse = diff * light.color;

        // specular
        float specularStrength = 0.5f;
        glm::vec3 view_dir = glm::normalize(fsp.view_pos - fsp.frag_pos);
        glm::vec3 reflect_dir = glm::reflect(-light_dir, norm);
        float spec =
            glm::pow(glm::max(glm::dot(view_dir, reflect_dir), 0.0f), 32.0f);
        glm::vec3 specular = specularStrength * spec * light.color;

        glm::vec3 obj_color =
            glm::vec3(fsp.obj_color.x, fsp.obj_color.y, fsp.obj_color.z);
        FragColor += (ambient + diffuse + specular) * obj_color;
    }

    return clamp(FragColor);
}

static glm::vec3 blinn_phong_helper(FragShader_Payload &fsp, glm::vec3 color)
{
    glm::vec3 ret = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 norm = glm::normalize(fsp.model_it * fsp.normal);
    for (auto light : fsp.lights) {
        // ambient
        float ambientStrength = 0.1f;
        glm::vec3 ambient = ambientStrength * light.color;

        // diffuse
        glm::vec3 light_dir = glm::normalize(light.pos - fsp.frag_pos);
        float diff = glm::max(glm::dot(norm, light_dir), 0.0f);
        glm::vec3 diffuse = diff * light.color;

        // specular
        float specularStrength = 0.5f;
        glm::vec3 view_dir = glm::normalize(fsp.view_pos - fsp.frag_pos);
        glm::vec3 reflect_dir = glm::reflect(-light_dir, norm);
        float spec =
            glm::pow(glm::max(glm::dot(view_dir, reflect_dir), 0.0f), 32.0f);
        glm::vec3 specular = specularStrength * spec * light.color;

        ret += (ambient + diffuse + specular) * color;
    }
    return ret;
}

glm::vec3 blinn_phong_obj_color(FragShader_Payload &fsp) {
    glm::vec3 FragColor = blinn_phong_helper(fsp, fsp.obj_color);

    return clamp(FragColor);
}

glm::vec3 blinn_phong_texture(FragShader_Payload &fsp) {
    glm::vec3 color =
        fsp.texture.value()->tex2D(fsp.tex_coords[0], fsp.tex_coords[1]);
    color = glm::pow(color, glm::vec3(2.2f));

    glm::vec3 FragColor = blinn_phong_helper(fsp, color);
    FragColor = glm::pow(FragColor, glm::vec3(1.0f / 2.2f));

    return clamp(FragColor);
}

glm::vec3 show_normal(FragShader_Payload &fsp)
{
    return glm::normalize(fsp.model_it * fsp.normal);
}
