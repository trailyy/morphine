#pragma once
#include "../proton/GameTimer.hpp"
#include "../proton/Console.hpp"
#include "../manager/ResourceManager.hpp"
#include "../proton/RTFont.hpp"
#include "../proton/Boost.hpp"
#include "../proton/Vector.hpp"
#include "../enums.hpp"

#include <deque>

#pragma pack(push, 1)
struct OSMessage
{
    eMessageType m_type;
    int m_param;
    Vector2 m_pos;
    Vector2 m_size;
    float m_font_size;
    std::string m_string;
    uint32_t m_param2;
    bool m_fullscreen;
    std::string m_string2;
    std::string m_string3;
};

class BaseApp
{
public:
	void* m_vtable;
	BoostSignal m_sig_input;
	BoostSignal m_sig_input_move;
	BoostSignal m_sig_os;
	BoostSignal m_sig_update;
	BoostSignal m_sig_render;
	BoostSignal m_sig_pre_enterbackground;
	BoostSignal m_sig_enterbackground;
	BoostSignal m_sig_enterforeground;
	BoostSignal m_sig_accel;
	BoostSignal m_sig_trackball;
	BoostSignal m_sig_arcade_input;
	BoostSignal m_sig_raw_keyboard;
	BoostSignal m_sig_hardware;
	BoostSignal m_sig_on_screen_size_changed;
	BoostSignal m_sig_unload_surfaces;
	BoostSignal m_sig_load_surfaces;
	BoostSignal m_sig_joypad_events;
	BoostSignal m_sig_native_input_state_changed;
	bool m_console_visible;
	bool m_fps_visible;
	bool m_initialized;
	pad(5);
	GameTimer m_game_timer;
	Console* m_console;
	RTFont m_small_font;
	RTFont m_large_font;
	RTFont m_fixed_font;
	RTFont m_growmoji_font;
	std::deque<OSMessage> m_os_messages;
	pad(8);
	ResourceManager m_resource_manager;
	pad(432);

	void AddOSMessage(OSMessage& m)
	{
		m_os_messages.push_back(m);
	}

	void SetFPSLimit(float fps)
	{
		OSMessage o;
		o.m_type = eMessageType::SET_FPS_LIMIT;
		o.m_pos.x = fps;

		this->AddOSMessage(o);
	}

	void SetVideoMode(int width, int height, bool fullscreen, float aspect_ratio)
	{
		OSMessage o;
		o.m_type = eMessageType::SET_VIDEO_MODE;
		o.m_pos.x = (float)width;
		o.m_pos.y = (float)height;
		o.m_fullscreen = fullscreen;
		o.m_font_size = aspect_ratio;

		this->AddOSMessage(o);
	}

	uint32_t GetTick()
	{
		return m_game_timer.m_game_timer;
	}
};
#pragma pack(pop)