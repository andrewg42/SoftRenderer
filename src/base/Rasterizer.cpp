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
    //std::cerr << "Add model: " << vec_models[vec_models.size() - 1]->m_name << '\n';
    //for(std::size_t i{}; i<vec_models[vec_models.size() - 1]->vec_meshes.size(); i++) {
    //    std::cerr << "\tmesh (" << i+1 << "): " << vec_models[vec_models.size() - 1]->vec_meshes[i].mesh_name << '\n';
    //}
}

void Rasterizer::add_texture(std::string const &path)
{
    std::shared_ptr<Texture> p_texture = std::make_shared<Texture>(path);
    p_private->vec_texture.push_back(p_texture);
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
    glm::mat4 ret(1.0f);
    ret[2][2] = -1.0f;
    return ret;
}

void Rasterizer::set_color(std::size_t idx, glm::u8vec4 color)
{
    if (idx < 0 || idx > m_buffer.m_color.size()) return;
    m_buffer.m_color[idx] = color;
}

void Rasterizer::set_color(std::size_t idx, glm::vec4 color) {
    return set_color(idx,
                     glm::u8vec4(
                        static_cast<unsigned char>(255 * color.x),
                        static_cast<unsigned char>(255 * color.y),
                        static_cast<unsigned char>(255 * color.z),
                        static_cast<unsigned char>(255 * color.w)
                     ));
}

void Rasterizer::set_color(std::size_t x, std::size_t y, glm::u8vec4 color)
{
    std::size_t idx = y * m_input.m_width * m_buffer.sample + x;
    set_color(idx, color);
}

void Rasterizer::tick() {
    ImGui_Context &gui = ImGui_Context::instance();
    p_private->update();
    if(gui.m_config.aa_type == AA_Type::MSAA) {
        m_buffer.sample = 2;
    } else {
        m_buffer.sample = 1;
    }
    m_buffer.resize(m_input.m_width * m_input.m_height);
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

static glm::vec3 ndc2scr(glm::vec3 &vec, std::size_t width, std::size_t height)
{
    glm::vec3 ret = vec;
    ret[0] = (ret[0] + 1.0f) * (width / 2);
    ret[1] = (ret[1] + 1.0f) * (height / 2);
    return ret;
}


void Rasterizer::draw()
{
    // initialization
    // --------------
    ImGui_Context &gui = ImGui_Context::instance();
    // TODO: switch frag_shader by option
    //switch(gui.m_config.aa_type) {
    //case AA_Type::None:
    //    frag_shader = blinn_phong_obj_color;
    //    break;
    //case AA_Type::FXAA:
        frag_shader = blinn_phong_texture;
    //    break;
    //case AA_Type::SMAA:
    //    frag_shader = show_normal;
    //    break;
    //default:
    //    frag_shader = blinn_phong_obj_color;
    //}

    // clear color
    m_buffer.clear(Buffer_Type::color | Buffer_Type::depth);

    // vertex shader
    // --------------
    // get transformation matrices
    glm::mat4 view = m_input.m_camera.get_lookAt_mat();
    glm::mat4 proj = m_input.get_proj();

    // TODO: more models?
    auto &vec_pos = p_private->m_scene.vec_models[0]->vec_meshes[0].pos;
    glm::mat4 modelview = view * model_mat();
    // initialize buffers. may be extracted into a class to manage the buffers?
    std::vector<glm::vec3> vec_pos_model;
    std::vector<glm::vec3> vec_pos_view;
    std::vector<glm::vec3> vec_pos_scr;
    vec_pos_model.reserve(vec_pos.size());
    vec_pos_view.reserve(vec_pos.size());
    vec_pos_scr.reserve(vec_pos.size());
    for (std::size_t i{}; i < vec_pos.size(); i++) {
        glm::vec4 pos_model = model_mat() * glm::vec4(vec_pos[i], 1.0f);
        vec_pos_model.push_back(perspective_divide(pos_model));
        glm::vec4 pos_view = modelview * glm::vec4(vec_pos[i], 1.0f);
        vec_pos_view.push_back(perspective_divide(pos_view));
        glm::vec4 pos_proj = proj * pos_view;
        vec_pos_scr.push_back(ndc2scr(perspective_divide(pos_proj),
                               m_input.m_width, m_input.m_height));
    }

    fsp.view_pos = m_input.m_camera.eye;
    // normal
    auto &vec_norm = p_private->m_scene.vec_models[0]->vec_meshes[0].norm;
    //bool normal_flag = true; // TODO: have some trouble with the processing of normal. Need to REWRITE the OBJ_Loader.
    std::vector<glm::vec3> vec_norm_view;
    vec_norm_view.reserve(vec_norm.size());
    for (std::size_t i{}; i < vec_norm.size(); i++) {
        glm::mat3 modelview_ti = glm::transpose(glm::inverse(glm::mat3(modelview)));
        vec_norm_view.push_back(modelview_ti * vec_norm[i]);
    }
    fsp.model_it = glm::transpose(glm::inverse(model_mat()));

    // texture
    auto &vec_tc = p_private->m_scene.vec_models[0]->vec_meshes[0].uv;
    fsp.texture = p_private->vec_texture[0];

    // TODO: more lights?
    fsp.lights.clear();
    Light light = p_private->m_scene.m_lights;
    fsp.lights.push_back(light);

    // shape assembly
    // ---------------
    // TODO: need multi-threads?
    for (auto face : p_private->m_scene.vec_models[0]->vec_meshes[0].face) {
        // face culling
        auto A_view = vec_pos_view[face[0]];
        auto B_view = vec_pos_view[face[1]];
        auto C_view = vec_pos_view[face[2]];

        glm::vec3 bc_view = (A_view + B_view + C_view) / 3.0f;
        glm::vec3 norm_view =
            glm::normalize(glm::cross(B_view - A_view, C_view - A_view));

        if (!gui.m_config.enable_faceculling &&
            gui.m_config.enable_faceculling &&
            glm::dot(bc_view, norm_view) > 0)
            continue;

        // fragment shader
        // ---------------
        glm::mat3 tri;
        {
            tri[0] = vec_pos_scr.at(face[0]);
            tri[1] = vec_pos_scr.at(face[1]);
            tri[2] = vec_pos_scr.at(face[2]);
        }

        // draw triangle
        {
            // find bounding box of current triangle
            int bbox_min[2] = {m_input.m_width - 1, m_input.m_height - 1};
            int bbox_max[2] = {0, 0};
            for (int i{}; i < 3; i++) {
                for (int j{}; j < 2; j++) {
                    bbox_min[j] =
                        std::min(bbox_min[j], static_cast<int>(tri[i][j]));
                    bbox_max[j] =
                        std::max(bbox_max[j], static_cast<int>(tri[i][j]));
                }
            }

            auto A = tri[0], B = tri[1], C = tri[2];
            auto A_model = vec_pos_model.at(face[0]);
            auto B_model = vec_pos_model.at(face[1]);
            auto C_model = vec_pos_model.at(face[2]);
            auto A_norm = vec_norm.at(face[0]);
            auto B_norm = vec_norm.at(face[1]);
            auto C_norm = vec_norm.at(face[2]);
            auto A_tc = vec_tc.at(face[0]);
            auto B_tc = vec_tc.at(face[1]);
            auto C_tc = vec_tc.at(face[2]);

            // int step = 1; // setting from AA
            // get coefficient of function barycentric
            // M = {AB, AC, PA}, where AB and AC are deterministic in a given
            // triangle
            glm::vec3 M_x = {B[0] - A[0], C[0] - A[0], 0.0f};
            glm::vec3 M_y = {B[1] - A[1], C[1] - A[1], 0.0f};

            if (gui.m_config.enable_wireframe) {
                draw_line(A, B);
                draw_line(B, C);
                draw_line(C, A);
                goto end_fs;
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
                 y <= std::min(bbox_max[1],
                               static_cast<int>(m_input.m_height - 1));
                 y++) {
                for (int x = std::max(bbox_min[0], 0);
                     x <= std::min(bbox_max[0],
                                   static_cast<int>(m_input.m_width - 1));
                     x++) {
                    // get u and v of barycentric
                    // u, v are used to calculate the color and depth at point
                    // (x+0.5, y+0.5)
                    M_x[2] = A[0] - x - 0.5f;
                    M_y[2] = A[1] - y - 0.5f;
                    glm::vec3 tmp = glm::cross(M_x, M_y);
                    float u = tmp.x / tmp.z;
                    float v = tmp.y / tmp.z;
                    if (u < 0 || v < 0 || u + v > 1.0f) // outside the triangle
                        continue;

                    std::size_t idx = y * m_input.m_width + x;

                    // depth testing
                    // this should behind shading in GPU, but this is CPU and moving this stage forward can improve performance...
                    float depth = u * (B[2] - A[2]) + v * (C[2] - A[2]) + A[2];
                    if (depth < m_buffer.m_depth[idx])
                        continue;
                    m_buffer.m_depth[idx] = depth;

                    // shading
                    fsp.obj_color = gui.m_config.obj_color; // TODO: texture
                    fsp.normal = u * (B_norm - A_norm) + v * (C_norm - A_norm) + A_norm;
                    fsp.frag_pos = u * (B_model - A_model) +
                                   v * (C_model - A_model) + A_model;
                    fsp.tex_coords = u * (B_tc - A_tc) + v * (C_tc - A_tc) + A_tc;

                    glm::vec3 result = frag_shader(fsp);

                    set_color(idx, glm::vec4(result, 1.0f));
                }
            }
        }
    end_fs:;
    }


    m_buffer.down_sampling(m_input.m_width, m_input.m_height);

    tick();

    // ping-pong buffer?
    // std::swap(m_canvas, m_buffer);
}
