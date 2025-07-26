#pragma once
#include "../hooks.hpp"
#include "renderer/renderer.hpp"
#include "../../menu/menu.hpp"
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_opengl3.h"
#include "../../lib/imgui/imgui_impl_win32.h"

using swap_buffers_t = BOOL(WINAPI*)(HDC);
BOOL WINAPI Hooked_SwapBuffers(HDC hdc)
{
	static auto orig = (swap_buffers_t)g_hooks->hkSwapBuffers.m_orig;

	if (!g_menu->initialize() && g_menu->m_initialized == false)
		return orig(hdc);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	g_menu->render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return orig(hdc);
}