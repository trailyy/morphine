#pragma once
#include "../../globals.hpp"
#include "../../lib/imgui/imgui_impl_win32.h"
#include "../../menu/menu.hpp"
#include "../hooks.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Hooked_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_KEYUP && (wparam == VK_INSERT || wparam == VK_DELETE || wparam == VK_HOME))
		g_menu->m_open = !g_menu->m_open;

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	ImGuiIO io = ImGui::GetIO();
	if (io.WantCaptureMouse && (msg == WM_RBUTTONDBLCLK || msg == WM_LBUTTONDBLCLK || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP || msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP || msg == WM_MOUSEWHEEL || msg == WM_MOUSEMOVE) || io.WantCaptureKeyboard)
		return true;

	return CallWindowProcA(g_globals->m_dll.m_wndproc, hwnd, msg, wparam, lparam);
}