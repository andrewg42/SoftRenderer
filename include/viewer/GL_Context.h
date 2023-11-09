#pragma once

#include "glm/glm.hpp"

#include "utils/check_gl.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "GLFW/glfw3.h"

#include "config/Config.h"
#include "utils/Singleton.h"


struct Shader {
    GLuint id;

    Shader() = default;
    explicit Shader(char const *vs_code, char const *fs_code)
    {
        // vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(
            vs, 1, &vs_code,
            NULL); // the third parameter accept const char* const* (aka char**)
        glCompileShader(vs);
        // check compiling error
        check_compile_errors(vs, "VERTEX");
        // fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fs_code, NULL);
        glCompileShader(fs);
        // check compiling error
        check_compile_errors(fs, "FRAGMENT");
        // link shaders
        id = glCreateProgram();
        glAttachShader(id, vs);
        glAttachShader(id, fs);
        glLinkProgram(id);
        // check linking error
        check_compile_errors(id, "PROGRAM");
        // delete source
        CHECK_GL(glDeleteShader(vs));
        CHECK_GL(glDeleteShader(fs));
    }
    ~Shader() { glUseProgram(0); }

    void use() const { glUseProgram(id); }

    void check_compile_errors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                          << "\n"
                          << infoLog
                          << "\n -- "
                             "-------------------------------------------------"
                             "-- -- \n";
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                          << "\n"
                          << infoLog
                          << "\n -- "
                             "-------------------------------------------------"
                             "-- -- \n";
            }
        }
    }
};


struct GL_Context: Singleton<GL_Context> {
    // GL states
    GLFWwindow *window;
    GLuint VAO, VBO, EBO;
    Shader shader;
    GLuint texture;

    GL_Context():
        window(nullptr)
    {}
    ~GL_Context() = default;

    void init();
    void register_cbs();
    void main_loop();
    void clean_up();

    void run();
};


