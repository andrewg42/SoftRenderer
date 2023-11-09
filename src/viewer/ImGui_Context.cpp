#include "viewer/ImGui_Context.h"

#include <string>

void ImGui_Context::init_imgui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // setup platform/renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((std::string("#version ") + std::to_string(VERSION)).c_str());

    // load font
    ImFont *font = io.Fonts->AddFontDefault();
    IM_ASSERT(font != nullptr);

    // states
    show_demo_window = false;
    show_another_window = false;
}


void ImGui_Context::start_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGui_Context::draw_panel()
{
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text.");

        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::Checkbox("Another Window", &show_another_window);

        // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        // Edit 3 floats representing a color
        ImGui::ColorEdit3("obj color", (float *)&m_config.obj_color);

        // Buttons return true when clicked (most widgets
        // return true when edited/activated)
        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        // calculate fps
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        float fps = io.Framerate;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / fps, fps);

        ImGui::End();
    }

    if (show_another_window) {
        ImGui::Begin(
            "Another Window",
            &show_another_window); // Pass a pointer to our bool variable (the
                                   // window will have a closing button that
                                   // will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}

void ImGui_Context::end_frame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


// Start the Dear ImGui frame
// ImGui_ImplOpenGL3_NewFrame();
// ImGui_ImplGlfw_NewFrame();
// ImGui::NewFrame();

// if (show_demo_window)
//     ImGui::ShowDemoWindow(&show_demo_window);
//
//// 2. Show a simple window that we create ourselves. We use a Begin/End pair
//// to create a named window.

//{
//    // 在 ImGui 窗口中添加一个按钮，用于触发文件对话框
//    if (ImGui::Button("Load File")) {
//        show_file_dialog = true;
//    }

//    // 在 ImGui 窗口中添加文件对话框
//    // 在 ImGui 窗口中添加文件对话框
//    if (show_file_dialog) {
//        ImGui::OpenPopup("File Dialog");
//        if (ImGui::BeginPopupModal("File Dialog", NULL,
//                                   ImGuiWindowFlags_AlwaysAutoResize)) {
//            ImGui::Text("Select a file:");

//            // 使用 ImGui 的输入框来获取文件名
//            ImGui::InputText("File Name", selected_file,
//                             sizeof(selected_file));

//            if (ImGui::Button("Open")) {
//                // 处理文件加载操作，根据 selected_file 执行加载操作
//                // ...

//                show_file_dialog = false;
//            }

//            if (ImGui::Button("Cancel")) {
//                show_file_dialog = false;
//            }

//            ImGui::EndPopup();
//        }
//    }
//}

//// 3. Show another simple window.
// if (show_another_window) {
//     ImGui::Begin(
//         "Another Window",
//         &show_another_window); // Pass a pointer to our bool variable (the
//                                // window will have a closing button that
//                                // will clear the bool when clicked)
//     ImGui::Text("Hello from another window!");
//     if (ImGui::Button("Close Me"))
//         show_another_window = false;
//     ImGui::End();
// }
