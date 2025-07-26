#pragma once
#include <Windows.h>
#include "../lib/minhook/hook.h"

struct hook_t
{
public:
	void hook(void* func_in, void* func_hook_in, bool force = false)
	{
		m_func = func_in;
		m_hook = func_hook_in;

		if ((!force && m_is_hooked) || !m_func)
			return;

		MH_CreateHook(m_func, m_hook, (void**)&m_orig);
		MH_EnableHook(m_func);

		m_is_hooked = true;
	}

	void hook(uintptr_t func_in, void* func_hook_in, bool force = false)
	{
		m_func = (void*)func_in;
		m_hook = func_hook_in;

		if ((!force && m_is_hooked) || !m_func)
			return;

		MH_CreateHook(m_func, m_hook, (void**)&m_orig);
		MH_EnableHook(m_func);

		m_is_hooked = true;
	}

	void clear()
	{
		if (!m_is_hooked || !m_func)
			return;

		MH_DisableHook(m_func);
		MH_RemoveHook(m_func);

		m_is_hooked = false;
	}

	auto is_hooked() const { return m_is_hooked; }

	void* m_orig;

private:
	void* m_func{};
	void* m_hook{};
	bool m_is_hooked{};
};

class c_hooks
{
public:
	void initialize();
	void shutdown();

	hook_t hkIsDebuggerPresent;
	hook_t hkCheckRemoteDebuggerPresent;

	hook_t hkSwapBuffers;

	hook_t hkUpdate;							// App::Update
	hook_t hkSetFPSLimit;						// BaseApp::SetFPSLimit
	hook_t hkLogMsg;							// LogMsg

	hook_t hkDialogIsOpened;					// GameLogicComponent::DialogIsOpened
	hook_t hkOnTextGameMessage;					// GameLogicComponent::OnTextGameMessage
	hook_t hkProcessTankUpdatePacket;			// GameLogicComponent::ProcessTankUpdatePacket
	hook_t hkHandleTouchAtWorldCoordinates;		// LevelTouchComponent::HandleTouchAtWorldComponent
	hook_t hkHandleTrackPacket;					// TrackHandlerComponent::HandleTrackPacket

	hook_t hkSendPacket;						// SendPacket
	hook_t hkSendPacketRaw;						// SendPacketRaw

	hook_t hkCanMessageT4;						// NetAvatar::CanMessageT4
	hook_t hkOnPunch;							// NetAvatar::OnPunch

	hook_t hkSerialize;							// Tile::Serialize
	hook_t hkCameraUpdate;						// WorldCamera::CameraUpdate
};

extern c_hooks* g_hooks;