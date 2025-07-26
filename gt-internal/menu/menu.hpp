#pragma once
#include "../globals.hpp"
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include "../lib/imgui/imgui_impl_win32.h"

class c_menu
{
public:
	bool m_initialized = false;
	bool m_open = true;
	ImVec2 m_screen_size = { };

	ImDrawList* m_drawlist = nullptr;

	bool initialize();
	void render();
};

extern c_menu* g_menu;