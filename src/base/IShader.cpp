#include "base/IShader.h"

#include <iostream>
#include "math/Mat_Op.h"

static glm::vec3 clamp(glm::vec3 color) {
    return glm::vec3(
                (color.x > 1.0f) ? 1.0f : color.x,
                (color.y > 1.0f) ? 1.0f : color.y,
                (color.z > 1.0f) ? 1.0f : color.z
           );
}

glm::vec3 blinn_phong_obj_color(FragShader_Payload &fsp) {
    glm::vec3 FragColor = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 norm = glm::normalize(fsp.model_it * fsp.normal);
    for(auto light : fsp.lights) {
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

glm::vec3 blinn_phong_texture(FragShader_Payload &fsp) {
    //std::size_t idx = static_cast<std::size_t>(fsp.tex_coords[1] * fsp.texture.value()->m_ny) * fsp.texture.value()->m_ny +
    //    static_cast<std::size_t>(fsp.tex_coords[0] * fsp.texture.value()->m_nx);
    std::size_t idx =
        static_cast<std::size_t>(fsp.tex_coords[0] *
                                 fsp.texture.value()->m_nx) *
            fsp.texture.value()->m_nx +
        static_cast<std::size_t>(fsp.tex_coords[1] * fsp.texture.value()->m_ny);

    glm::u8vec3 color = *(fsp.texture.value()->data() + idx); // TODO: mipmap, area query
    glm::vec3 color_float = glm::vec3(
        static_cast<float>(color.x) / 255.0f,
        static_cast<float>(color.y) / 255.0f,
        static_cast<float>(color.z) / 255.0f
        );
    //glm::vec3 FragColor = glm::vec3(0.0f, 0.0f, 0.0f);
    //glm::vec3 norm = glm::normalize(fsp.model_it * fsp.normal);
    //for (auto light : fsp.lights) {
    //    float ambientStrength = 0.1f;
    //    glm::vec3 ambient = ambientStrength * light.color;

    //    // diffuse
    //    glm::vec3 light_dir = glm::normalize(light.pos - fsp.frag_pos);
    //    float diff = glm::max(glm::dot(norm, light_dir), 0.0f);
    //    glm::vec3 diffuse = diff * light.color;

    //    // specular
    //    float specularStrength = 0.5f;
    //    glm::vec3 view_dir = glm::normalize(fsp.view_pos - fsp.frag_pos);
    //    glm::vec3 reflect_dir = glm::reflect(-light_dir, norm);
    //    float spec =
    //        glm::pow(glm::max(glm::dot(view_dir, reflect_dir), 0.0f), 32.0f);
    //    glm::vec3 specular = specularStrength * spec * light.color;

    //    FragColor += (ambient + diffuse + specular) * color;
    //}

    return clamp(color_float);
}

glm::vec3 show_normal(FragShader_Payload &fsp)
{
    return glm::normalize(fsp.model_it * fsp.normal);
}