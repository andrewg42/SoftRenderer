file(GLOB imgui_sources CONFIGURE_DEPENDS
    ${imgui_SOURCE_DIR_}/*.cpp
    ${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.cpp
)

add_library(imgui STATIC ${imgui_sources})

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR_} ${imgui_SOURCE_DIR_}/backends)

target_link_libraries(imgui PUBLIC glfw glad)
