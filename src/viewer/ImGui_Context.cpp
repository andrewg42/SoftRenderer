#include "viewer/ImGui_Context.h"

#include <string>

bool ImGui::Combo(const char *label, int *current_item,
                  const char *const items[], int items_count,
                  int height_in_items);

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
}


void ImGui_Context::start_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

static void show_debugging_panel() {
    //MY_DEMO_MARKER("Debug");
    if (ImGui::CollapsingHeader("Debugging")) {
        ImGuiIO &io = ImGui::GetIO();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse pos: <INVALID>");
        ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
        ImGui::Text("Mouse down:");
        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            if (ImGui::IsMouseDown(i)) {
                ImGui::SameLine();
                ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
            }
        ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

        // We iterate both legacy native range and named ImGuiKey ranges, which
        // is a little odd but this allows displaying the data for old/new
        // backends. User code should never have to go through such hoops! You
        // can generally iterate between ImGuiKey_NamedKey_BEGIN and
        // ImGuiKey_NamedKey_END.
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
        struct funcs {
            static bool IsLegacyNativeDupe(ImGuiKey) { return false; }
        };
        ImGuiKey start_key = ImGuiKey_NamedKey_BEGIN;
#else
        struct funcs {
            static bool IsLegacyNativeDupe(ImGuiKey key)
            {
                return key < 512 && ImGui::GetIO().KeyMap[key] != -1;
            }
        }; // Hide Native<>ImGuiKey duplicates when both exists in the array
        ImGuiKey start_key = (ImGuiKey)0;
#endif
        ImGui::Text("Keys down:");
        for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END;
             key = (ImGuiKey)(key + 1)) {
            if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))
                continue;
            ImGui::SameLine();
            ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\""
                                                        : "\"%s\" %d",
                        ImGui::GetKeyName(key), key);
        }
        ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "",
                    io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "",
                    io.KeySuper ? "SUPER " : "");
        ImGui::Text("Chars queue:");
        for (int i = 0; i < io.InputQueueCharacters.Size; i++) {
            ImWchar c = io.InputQueueCharacters[i];
            ImGui::SameLine();
            ImGui::Text("\'%c\' (0x%04X)",
                        (c > ' ' && c <= 255) ? (char)c : '?', c);
        } // FIXME: We should convert 'c' to UTF-8 here but the functions are
          // not public.
    }
}


void ImGui_Context::draw_panel()
{


    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Configuration");

        // calculate fps
        float fps = io.Framerate;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / fps, fps);

        {
            ImGui::ColorEdit4("obj color", (float *)&m_config.obj_color);
            ImGui::ColorEdit3("light color", (float *)&m_config.light_color);
            ImGui::ColorEdit4("clear color", (float *)&m_config.clear_color);
        }

        {
            ImGui::Checkbox("Face Culling",
                            &m_config.enable_faceculling);
            ImGui::Checkbox("Wireframe", &m_config.enable_wireframe);
        }

        static const char *model_items[] = {"light", "normal", "texture"};
        static int model_item = static_cast<int>(m_config.model_type);
        ImGui::Combo("Model", &model_item, model_items,
                        IM_ARRAYSIZE(model_items));
        m_config.model_type = static_cast<Model_Type>(model_item);
        if(m_config.model_type==Model_Type::light) {
            {
                static const char *aa_items[] = {
                    "None", "MSAA", "FXAA(still not supported)",
                    "SMAA(still not supported)"};
                static int aa_item = static_cast<int>(m_config.aa_type);
                ImGui::Combo("Anti-aliasing", &aa_item, aa_items,
                                IM_ARRAYSIZE(aa_items));
                m_config.aa_type = static_cast<AA_Type>(aa_item);
            }
            {
                static const char *ao_items[] = {"None", "SSAO"};
                static int ao_item = static_cast<int>(m_config.ao_type);
                ImGui::Combo("Ambient Occlusion", &ao_item, ao_items,
                                IM_ARRAYSIZE(ao_items));
                m_config.ao_type = static_cast<AO_Type>(ao_item);
            }
            {
                static const char *shadow_items[] = {
                    "None", "Shadow Mapping", "PCSS"};
                static int shadow_item =
                    static_cast<int>(m_config.shadow_type);
                ImGui::Combo("Shadow", &shadow_item, shadow_items,
                                IM_ARRAYSIZE(shadow_items));
                m_config.shadow_type =
                    static_cast<Shadow_Type>(shadow_item);
            }
        }
        

        //std::cerr << "AA: " << static_cast<int>(m_config.aa_type) << ", "
        //        << "AO: " << static_cast<int>(m_config.ao_type) << ", "
        //        << "Shadow: " << static_cast<int>(m_config.shadow_type) << "\n";

        show_debugging_panel();

        ImGui::End();
    }

}

void ImGui_Context::end_frame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}