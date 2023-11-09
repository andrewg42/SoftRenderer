if(NOT TARGET glm)
    add_subdirectory(glm)
    set_target_properties(glm PROPERTIES FOLDER ${THIRD_PARTY}/glm)
endif()
target_link_libraries(main PUBLIC glm)

if(NOT TARGET glfw)
    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${THIRD_PARTY}/glfw)
endif()
target_link_libraries(main PUBLIC glfw)

if(NOT TARGET glad)
    add_subdirectory(glad)
    set_target_properties(glad PROPERTIES FOLDER ${THIRD_PARTY}/glad)
endif()
target_link_libraries(main PUBLIC glad)

if(NOT TARGET imgui)
    set(imgui_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking)
    include(imgui.cmake)
    set_target_properties(imgui PROPERTIES FOLDER ${imgui_SOURCE_DIR_})
endif()
target_link_libraries(main PUBLIC imgui)

if(NOT TARGET stb)
    set(stb_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/stb)
    include(stb.cmake)
    set_target_properties(stb PROPERTIES FOLDER ${stb_SOURCE_DIR_})
endif()


target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/OBJ-Loader/Source)