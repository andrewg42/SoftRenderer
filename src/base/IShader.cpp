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

glm::vec3 blinn_phong_obj_color(FragShader_Payload &fsp) {
    glm::vec3 FragColor = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 norm = glm::normalize(fsp.model_it * fsp.normal);
    for(auto light : fsp.lights) {
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
        glm::vec3 h = glm::normalize(view_dir + reflect_dir);
        float spec = glm::pow(glm::max(glm::dot(norm, h), 0.0f), 32.0f);
        glm::vec3 specular = specularStrength * spec * light.color;

        glm::vec3 obj_color =
            glm::vec3(fsp.obj_color.x, fsp.obj_color.y, fsp.obj_color.z);
        FragColor += (ambient + diffuse + specular) * obj_color;
    }

    return clamp(FragColor);
}

glm::vec3 blinn_phong_texture(FragShader_Payload &fsp) {
    glm::vec3 color =
        fsp.texture.value()->get_data(fsp.tex_coords[0], fsp.tex_coords[1]);
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
        glm::vec3 h = glm::normalize(view_dir + reflect_dir);
        float spec = glm::pow(glm::max(glm::dot(norm, h), 0.0f), 32.0f);
        glm::vec3 specular = specularStrength * spec * light.color;

        FragColor += (ambient + diffuse + specular) * color;
    }

    return clamp(color);
}

glm::vec3 show_normal(FragShader_Payload &fsp)
{
    return glm::normalize(fsp.model_it * fsp.normal);
}