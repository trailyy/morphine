#pragma once
#include <cstdint>
#include "../proton/Vector.hpp"

class WorldRenderer;

#pragma pack(push, 1)
class WorldCamera
{
public:
	void* m_vtable;
	WorldRenderer* m_world_renderer;
	Vector2 m_camera_pos;
	Vector2 m_local_player_pos;
	pad(8);
	Vector2 m_zoom_level;
	pad(8);
	Vector2 m_screen_size;
	Vector2 m_zoomed_screen_size;
	pad(2);

	Vector2 world_to_screen(Vector2 pos) const
	{
		pos -= m_camera_pos;
		pos *= m_zoom_level;
		return pos;
	}

	Vector2 screen_to_world(Vector2 pos) const
	{
		pos /= m_zoom_level;
		pos += m_camera_pos;
		return pos;
	}
};

#pragma pack(pop)
