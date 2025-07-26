#pragma once
#include "../hooks.hpp"

void __cdecl Hooked_SetFPSLimit(BaseApp* base_app, float fps)
{
	static auto orig = reinterpret_cast<decltype(g_sdk->m_set_fps_limit_fn)>(g_hooks->hkSetFPSLimit.m_orig);

	print(_("setting fps limit %.f"), fps);

	orig(base_app, 0.0f);
}