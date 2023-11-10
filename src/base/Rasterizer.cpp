#include "base/Rasterizer.h"

#include "glm/glm.hpp"
#include <cmath>
#include <iostream>
#include <limits> // std::numeric_limits<T>::epsilon()

#include "base/Manager.h"
#include "utils/ticktock.h"
#include "viewer/ImGui_Context.h"

struct Rasterizer::Private : Manager {};

Rasterizer::Rasterizer():
    p_private(std::make_unique<Private>())
{
}

Rasterizer::~Rasterizer() = default;

void Rasterizer::set_buffer_size(std::size_t const w = SCR_WIDTH,
                                 std::size_t const h = SCR_HEIGHT)
{
    m_input.m_width = w;
    m_input.m_height = h;
    m_buffer.resize(w * h);
}

void Rasterizer::add_model(std::string const &path)
{
    auto &vec_models = p_private->m_scene.vec_models;
    vec_models.push_back(std::make_shared<Model>());
    vec_models[vec_models.size() - 1]->load_obj(path);
    std::cerr << "Add model: " << vec_models[vec_models.size() - 1]->m_name << '\n';
    for(std::size_t i{}; i<vec_models[vec_models.size() - 1]->vec_meshes.size(); i++) {
        std::cerr << "\tmesh (" << i+1 << "): " << vec_models[vec_models.size() - 1]->vec_meshes[i].mesh_name << '\n';
    }
}

char *Rasterizer::data()
{
    return reinterpret_cast<char *>(m_buffer.m_data.data());
}

static glm::vec3 perspective_divide(glm::vec4 pos)
{
    return glm::vec3(pos.x / pos.w, pos.y / pos.w, pos.z / pos.w);
}

static glm::mat4 model_mat()
{
    // TODO: for each model
    glm::mat4 ret(1);
    return ret;
}

void Rasterizer::set_color(std::size_t idx, glm::u8vec4 color)
{
    // std::cout << __func__ << ": idx = " << idx << '\n';
    if (idx < 0 || idx > m_buffer.m_color.size()) return;
    m_buffer.m_color[idx] = color;
}

void Rasterizer::set_color(std::size_t x, std::size_t y, glm::u8vec4 color)
{
    std::size_t idx = y * m_input.m_width * m_buffer.sample + x;
    set_color(idx, color);
}

void Rasterizer::tick() {
    p_private->update();
}

// Bresenham's line drawing algorithm (for debugging)
void Rasterizer::draw_line(glm::vec3 &begin, glm::vec3 &end)
{
    auto x1 = begin.x;
    auto y1 = begin.y;
    auto x2 = end.x;
    auto y2 = end.y;

    glm::u8vec4 line_color = {0, 0, 0, 255};

    int dx = x2 - x1;
    int dy = y2 - y1;
    int dx1 = std::fabs(dx);
    int dy1 = std::fabs(dy);
    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;

    int x, y;
    if (dy1 <= dx1) {
        int xe;
        if (dx >= 0) {
            x = x1;
            y = y1;
            xe = x2;
        }
        else {
            x = x2;
            y = y2;
            xe = x1;
        }
        set_color(x, y, line_color);
        for (x++; x < xe; x++) {
            if (px < 0)
                px = px + 2 * dy1;
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    y = y + 1;
                }
                else {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            set_color(x, y, line_color);
        }
    }
    else {
        int ye;
        if (dy >= 0) {
            x = x1;
            y = y1;
            ye = y2;
        }
        else {
            x = x2;
            y = y2;
            ye = y1;
        }
        set_color(x, y, line_color);
        for (y++; y < ye; y++) {
            if (py <= 0) {
                py = py + 2 * dx1;
            }
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    x = x + 1;
                }
                else {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            set_color(x, y, line_color);
        }
    }
}

void Rasterizer::draw_triangle(glm::mat3 &tri, glm::vec3 norm)
{
    // find bounding box of current triangle
    int bbox_min[2] = {m_input.m_width - 1, m_input.m_height - 1};
    int bbox_max[2] = {0, 0};
    for (int i{}; i < 3; i++) {
        for (int j{}; j < 2; j++) {
            bbox_min[j] = std::min(bbox_min[j], static_cast<int>(tri[i][j]));
            bbox_max[j] = std::max(bbox_max[j], static_cast<int>(tri[i][j]));
        }
    }

    auto A = tri[0], B = tri[1], C = tri[2];
    // int step = 1; // setting from AA
    // get coefficient of function barycentric
    // M = {AB, AC, PA}, where AB and AC are deterministic in a given triangle
    glm::vec3 M_x = {B[0] - A[0], C[0] - A[0], 0.0f};
    glm::vec3 M_y = {B[1] - A[1], C[1] - A[1], 0.0f};

    ImGui_Context &gui = ImGui_Context::instance();

    if (gui.m_config.enable_wireframe) {
        draw_line(A, B);
        draw_line(B, C);
        draw_line(C, A);
        return;
    }

    glm::vec4 tmp = gui.m_config.obj_color;
    glm::u8vec4 color =
        glm::u8vec4(static_cast<unsigned char>(tmp.x * 255),
                    static_cast<unsigned char>(tmp.y * 255),
                    static_cast<unsigned char>(tmp.z * 255),
                    static_cast<unsigned char>(tmp.w * 255));

    // omp_set_num_threads(16);
    // #pragma omp parallel for
    for (int y = std::max(bbox_min[1], 0);
         y <= std::min(bbox_max[1], static_cast<int>(m_input.m_height - 1));
         y++) {
        for (int x = std::max(bbox_min[0], 0);
             x <= std::min(bbox_max[0], static_cast<int>(m_input.m_width - 1));
             x++) {
            // get u and v of barycentric
            // u, v are used to calculate the color and depth at point (x, y)
            M_x[2] = A[0] - x - 0.5;
            M_y[2] = A[1] - y - 0.5;
            glm::vec3 tmp = glm::cross(M_x, M_y);
            float u = tmp.x / tmp.z;
            float v = tmp.y / tmp.z;
            if (u < 0 || v < 0 || u + v > 1.0f) // outside the triangle
                continue;

            std::size_t idx = y * m_input.m_width + x;

            // depth testing
            float depth = u * (B[2] - A[2]) + v * (C[2] - A[2]) + A[2];
            if (depth < m_buffer.m_depth[idx])
                continue;
            m_buffer.m_depth[idx] = depth;

            glm::vec3 frag_pos = u * (B - A) + v * (C - A) + A;

            // shading
            glm::vec3 light_color = p_private->m_scene.m_lights.color;
            // ambient
            float ambientStrength = 0.1f;
            glm::vec3 ambient = ambientStrength * light_color;

            // diffuse
            norm = glm::normalize(norm);
            glm::vec3 light_dir =
                glm::normalize(p_private->m_scene.m_lights.pos - frag_pos);
            float diff = glm::max(glm::dot(norm, light_dir), 0.0f);
            glm::vec3 diffuse = diff * light_color;

            // specular
            /*float specularStrength = 0.5;
            glm::vec3 view_dir = glm::normalize(p_private->m_camera.eye - FragPos);
            glm::vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            glm::vec3 specular = specularStrength * spec * lightColor;*/

            glm::vec3 obj_color = glm::vec3(color.x, color.t, color.z);
            glm::vec3 result = (ambient + diffuse) * obj_color;

            set_color(idx, glm::vec4(result, 1.0f));
        }
    }

}

static void ndc2scr(glm::vec3 &vec, std::size_t width, std::size_t height)
{
    vec[0] = (vec[0] + 1.0f) * (width / 2);
    vec[1] = (vec[1] + 1.0f) * (height / 2);
}

static glm::vec3 get_norm(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;
    return glm::cross(AB, AC);
}

void Rasterizer::draw()
{
    ImGui_Context &gui = ImGui_Context::instance();
    // clear color
    m_buffer.clear(Buffer_Type::color | Buffer_Type::depth);

    // vertex shader: transform the vertices
    glm::mat4 modelview = m_input.m_camera.get_lookAt_mat() * model_mat();
    glm::mat4 proj = m_input.get_proj();

    auto vec_pos = p_private->m_scene.vec_models[0]->vec_meshes[0].pos;
    // TODO: need multi-threads
    for (auto face : p_private->m_scene.vec_models[0]->vec_meshes[0].face) {
        // vertex shader
        // All vertices should be transform in one patch. But this is on cpu...
        glm::mat3x4 M = glm::mat3x4{glm::vec4(vec_pos[face[0]], 1.0f),
                                    glm::vec4(vec_pos[face[1]], 1.0f),
                                    glm::vec4(vec_pos[face[2]], 1.0f)};
        M = modelview * M;


        // face culling
        glm::vec3 A = perspective_divide(M[0]);
        glm::vec3 B = perspective_divide(M[1]);
        glm::vec3 C = perspective_divide(M[2]);
        glm::vec3 bc = (A + B + C) / 3.0f;
        glm::vec3 norm = get_norm(A, B, C);
        if (!gui.m_config.enable_faceculling && gui.m_config.enable_faceculling && glm::dot(bc, norm) > 0)
            continue;

        M = proj * M;
        A = perspective_divide(M[0]);
        B = perspective_divide(M[1]);
        C = perspective_divide(M[2]);

        ndc2scr(A, m_input.m_width, m_input.m_height);
        ndc2scr(B, m_input.m_width, m_input.m_height);
        ndc2scr(C, m_input.m_width, m_input.m_height);

        // fragment shader
        glm::mat3 tri = {A, B, C};

        draw_triangle(tri, norm);
    }


    m_buffer.down_sampling(m_input.m_width, m_input.m_height);

    tick();
    // std::swap(m_canvas, m_buffer); // ping-pong buffer?
}
