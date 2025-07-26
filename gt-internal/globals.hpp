#pragma once
#include "sdk/sdk.hpp"
#include "lib/xorstr/xorstr.hpp"

class c_globals
{
public:
	struct dll_data_t
	{
		int m_magic;
		HMODULE m_self;
		HWND m_window;
		WNDPROC m_wndproc;
		bool m_unload;
	} m_dll;

	struct game_t
	{
		struct character_state_t
		{
			ePlayerFlags m_flags = ePlayerFlags::NONE;

			float m_water_speed = 125.f;
			float m_acceleration_speed = 0.f;
			float m_punch_strength = 300.f;
			float m_speed = 250.f;
			float m_gravity = 1000.f;

			uint8_t m_punch_effect = 0;
			uint8_t m_build_range = 2; // * 32
			uint8_t m_punch_range = 2;

			int m_pupil_color = 0;
			int m_hair_color = 0;
			int m_eye_color = 0;
		} m_state;

		std::vector<NetAvatar*> m_cheat_users;
	} m_game;

	uintptr_t m_base_address;
};

extern c_globals* g_globals;