#pragma once
#include "utils/check_gl.h"
#include "GLFW/glfw3.h"

#include "utils/Singleton.h"

enum class Mouse_Button_Pressed : unsigned char {
	None = 0,
	LMB,
	MMB,
};

//struct Key_Binding: Singleton<Key_Binding> {
//    Mouse_Button_Pressed last_mb = Mouse_Button_Pressed::None;
//
//	void is_scrolled(GLFWwindow *window) {
//	}
//	void is_pressed(GLFWwindow *window) {
//	}
//};