#pragma once
#include "renderer.hpp"
#include "../../../globals.hpp"
#include "../../../utils/utils.hpp"

c_renderer* g_renderer = new c_renderer();

void c_renderer::initialize()
{
	print(_("initializing renderer for opengl"));

	// AppClass might be common, so lets search for both the window class and window name
	g_globals->m_dll.m_window = FindWindowA(_("AppClass"), _("Growtopia"));

	c_module opengl32_dll(_("opengl32.dll"));
	m_swap_buffers = opengl32_dll.get_proc_address(_("wglSwapBuffers")).get<uintptr_t>();

	if (!m_swap_buffers)
	{
		print(_("failed to find wglSwapBuffers!opengl32.dll"));
		return;
	}

	print(_("found wglSwapBuffers @ 0x%llx"), m_swap_buffers);
}