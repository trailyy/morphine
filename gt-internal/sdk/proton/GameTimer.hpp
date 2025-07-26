#pragma once
#include <cstdint>
#include <deque>

#pragma pack(push, 1)
class GameTimer
{
public:
	void* m_vtable;
	uint32_t m_last_time_ms;
	uint32_t m_time_ms; // 12
	uint32_t m_fps_timer; // 16
	uint32_t m_game_timer; // 20
	uint32_t m_shadow_offset;
	int m_fps;
	int m_fps_temp;
	bool m_game_timer_paused;
	int m_delta_ms;
	float m_delta_float;
	std::deque<float> m_tick_history;
	uint32_t m_shadow_game_tick;
	pad(7);
};
#pragma pack(pop)