#include "viewer/GL_Context.h"

#include <string>
#include <stdexcept>
#include "GLFW/glfw3.h"

#include "base/Rasterizer.h"
#include "viewer/ImGui_Context.h"
#include "config/Key_Binding.h"

static Rasterizer &raster = Rasterizer::instance();

static const std::string vs_code{R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}
)"};

static const std::string fs_code{R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D uTexture;

void main() {
    vec3 col = texture(uTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}
)"};

static void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static void framebuffer_size_cb(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    raster.m_input.scr_width = width;
    raster.m_input.scr_height = height;
}

static double last_xpos, last_ypos;
static Mouse_Button_Pressed last_mb = Mouse_Button_Pressed::None;

static void mouse_pos_cb(GLFWwindow *window, double xpos, double ypos)
{
    Mouse_Button_Pressed cur_mb = Mouse_Button_Pressed::None;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        cur_mb = Mouse_Button_Pressed::LMB;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        cur_mb = Mouse_Button_Pressed::MMB;
    }
    else {
        cur_mb = Mouse_Button_Pressed::None;
    }

    switch(cur_mb) {
    case Mouse_Button_Pressed::LMB:
        // mouse movements implement rotation
        raster.m_input.rotate(
            static_cast<float>(xpos), static_cast<float>(ypos),
            static_cast<float>(last_xpos), static_cast<float>(last_ypos)
        );
        break;
    case Mouse_Button_Pressed::MMB:
        // mouse movements implement movement
        raster.m_input.translate(
            static_cast<float>(xpos), static_cast<float>(ypos),
            static_cast<float>(last_xpos), static_cast<float>(last_ypos)
        );
        break;
    default:
        break;
    }

    last_mb = cur_mb;
    last_xpos = xpos;
    last_ypos = ypos;
}

static void scroll_cb(GLFWwindow *window, double x_offset, double y_offset) {
    //std::cerr << "x_offset: " << x_offset << ", y_offset: " << y_offset << '\n';
    raster.m_input.zoom(y_offset);
}

void GL_Context::register_cbs()
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
    glfwSetCursorPosCallback(window, mouse_pos_cb);
    glfwSetScrollCallback(window, scroll_cb);
}

void GL_Context::init()
{
    // glfw: intialize and configure
    if(!glfwInit()) {
        const char *errmsg;
        glfwGetError(&errmsg);
        if(!errmsg) errmsg = "(no error)";
        std::string msg = std::string("Failed to intialize GLFW: ")
                        + std::string(errmsg);
        throw std::runtime_error(msg);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION / 100);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (VERSION / 10) % 10);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Software Renderer", NULL, NULL);
    if (!window) {
        const char *errmsg;
        glfwGetError(&errmsg);
        if(!errmsg) errmsg = "(no error)";
        std::string msg = std::string("Failed to create GLFW window: ")
                        + std::string(errmsg);
        glfwTerminate();
        throw std::runtime_error(msg);
    }
    glfwMakeContextCurrent(window);
    register_cbs();
    glfwSwapInterval(1); // Enable vsync

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::string msg = std::string("Failed to intialize GLAD");
        glfwTerminate();
        throw std::runtime_error(msg);
    }

    // initialize ImGui
    ImGui_Context &gui = ImGui_Context::instance();
    gui.init_imgui(window);

    // initialize screen shader
    shader = Shader(vs_code.c_str(), fs_code.c_str());

    // initialize VAO, VBO, EBO
    float const vertices[] = {
        // positions    // texture coords
        1.0f,  1.0f,  1.0f, 1.0f, // top right
        1.0f,  -1.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f,  0.0f, 1.0f  // top left
    };
    unsigned int const indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    CHECK_GL(glGenVertexArrays(1, &VAO));
    CHECK_GL(glBindVertexArray(VAO));

    CHECK_GL(glGenBuffers(1, &VBO));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                          GL_STATIC_DRAW));

    CHECK_GL(glGenBuffers(1, &EBO));
    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    CHECK_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                          GL_STATIC_DRAW));

    // position attribute
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                                   (void *)0));
    CHECK_GL(glEnableVertexAttribArray(0));
    // texture coord attribute
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                                   (void *)(2 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(1));

    // initialize rasterizer
    raster.add_model("assets/monkey/monkey.obj");
    raster.set_buffer_size(SCR_WIDTH, SCR_HEIGHT);

    // create a texture
    // -------------------------
    CHECK_GL(glGenTextures(1, &texture));

    CHECK_GL(glBindTexture(GL_TEXTURE_2D, texture));
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, raster.m_input.m_width,
                          raster.m_input.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                          raster.data()));

    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

}

void GL_Context::main_loop() {
    // handle input
    process_input(window);

    // initialization
    ImGui_Context &gui = ImGui_Context::instance();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // begin gui frame
    gui.start_frame();

    // draw gui panel
    gui.draw_panel();

    // render
    raster.draw();
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, texture));
    CHECK_GL(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, raster.m_input.m_width,
                             raster.m_input.m_height, GL_RGBA, GL_UNSIGNED_BYTE,
                             raster.data()));
    shader.use();
    CHECK_GL(glBindVertexArray(VAO));
    CHECK_GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    
    // end gui frame
    gui.end_frame();

 
    CHECK_GL(glfwSwapBuffers(window));
    CHECK_GL(glfwPollEvents());
}

void GL_Context::clean_up() {
    CHECK_GL(glDeleteVertexArrays(1, &VAO));
    CHECK_GL(glDeleteBuffers(1, &VBO));
    CHECK_GL(glDeleteBuffers(1, &EBO));

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void GL_Context::run()
{
    try {
        init();
    } catch(std::exception const &e){
        std::cerr << e.what() << '\n';
        return;
    }

    while (!glfwWindowShouldClose(window)) {
        main_loop();
    }

    clean_up();
}
