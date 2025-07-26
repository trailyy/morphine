#pragma once
#include "hooks.hpp"
#include "../globals.hpp"
#include "../utils/utils.hpp"

// hooks
#include "render/SwapBuffers.hpp"
#include "render/WndProc.hpp"

#include "app/App_Update.hpp"
#include "app/BaseApp_SetFPSLimit.hpp"
#include "app/LogMsg.hpp"

#include "component/GameLogicComponent_DialogIsOpened.hpp"
#include "component/GameLogicComponent_OnTextGameMessage.hpp" // missing
#include "component/GameLogicComponent_ProcessTankUpdatePacket.hpp"
#include "component/LevelTouchComponent_HandleTouchAtWorldCoordinates.hpp"
#include "component/TrackHandlerComponent_HandleTrackPacket.hpp"

#include "network/SendPacket.hpp"
#include "network/SendPacketRaw.hpp"

#include "player/NetAvatar_CanMessageT4.hpp" // missing
#include "player/NetAvatar_OnPunch.hpp" // missing

#include "winapi/IsDebuggerPresent.hpp"
#include "winapi/CheckRemoteDebuggerPresent.hpp"

#include "world/Tile_Serialize.hpp" // missing
#include "world/WorldCamera_CameraUpdate.hpp" // missing

c_hooks* g_hooks = new c_hooks();

void c_hooks::initialize()
{
	// initialize minhook... duh...
	MH_Initialize();

	hkSwapBuffers.hook(g_renderer->get_swap_buffers(), Hooked_SwapBuffers);
	print(_("hooked swapbuffers"));

	c_module ntdll_dll = c_module(_("ntdll.dll"));
	hkIsDebuggerPresent.hook(ntdll_dll.get_proc_address(_("IsDebuggerPresent")).get(), Hooked_IsDebuggerPresent);
	hkCheckRemoteDebuggerPresent.hook(ntdll_dll.get_proc_address(_("CheckRemoteDebuggerPresent")).get(), Hooked_CheckRemoteDebuggerPresent);

	// hook our game functions
	hkUpdate.hook(g_sdk->m_app_update_fn, Hooked_Update);
	hkSetFPSLimit.hook(g_sdk->m_set_fps_limit_fn, Hooked_SetFPSLimit);
	hkLogMsg.hook(g_sdk->m_log_msg_fn, Hooked_LogMsg);

	hkDialogIsOpened.hook(g_sdk->m_dialog_is_opened_fn, Hooked_DialogIsOpened);
	hkOnTextGameMessage.hook(g_sdk->m_on_text_game_message_fn, Hooked_OnTextGameMessage);
	hkProcessTankUpdatePacket.hook(g_sdk->m_process_tank_update_packet_fn, Hooked_ProcessTankUpdatePacket);
	hkHandleTouchAtWorldCoordinates.hook(g_sdk->m_handle_touch_at_world_coordinates_fn, Hooked_HandleTouchAtWorldCoordinates);
	hkHandleTrackPacket.hook(g_sdk->m_handle_track_packet_fn, Hooked_HandleTrackPacket);

	hkSendPacket.hook(g_sdk->m_send_packet_fn, Hooked_SendPacket);
	hkSendPacketRaw.hook(g_sdk->m_send_packet_raw_fn, Hooked_SendPacketRaw);

	hkCanMessageT4.hook(g_sdk->m_can_message_t4_fn, Hooked_CanMessageT4);

	// "hook" wndproc
	g_globals->m_dll.m_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(g_globals->m_dll.m_window, GWLP_WNDPROC, (LONG_PTR)Hooked_WndProc));

	print(_("hooked everything =)"));
}

void c_hooks::shutdown()
{
	// restore the wndproc to the original one
	SetWindowLongPtrA(g_globals->m_dll.m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_globals->m_dll.m_wndproc));

	hkSwapBuffers.clear();

	hkUpdate.clear();
	hkSetFPSLimit.clear();
	hkLogMsg.clear();

	hkDialogIsOpened.clear();
	hkOnTextGameMessage.clear();
	hkProcessTankUpdatePacket.clear();
	hkHandleTouchAtWorldCoordinates.clear();
	hkHandleTrackPacket.clear();

	hkSendPacket.clear();
	hkSendPacketRaw.clear();

	// cleanup
	auto ctx = ImGui::GetCurrentContext();
	if (ctx)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	// do i even have to comment this
	MH_Uninitialize();

	print(_("unhooked everything =("));
}