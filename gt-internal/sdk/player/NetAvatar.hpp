#pragma once
#include <cstdint>
#include <memory>
#include "../proton/Vector.hpp"
#include "../proton/EncryptedFloat.hpp"
#include "../enums.hpp"

#pragma pack(push, 1)
class NetAvatar
{
public:
	void* m_vtable;
	Vector2 m_pos;
	Vector2 m_size;
	pad(16);
	std::string m_name;
	int m_net_id;
	pad(21);
	bool m_facing_left;
	pad(110);
	bool m_jumping;
	pad(15);
	bool m_on_solid;
	bool m_sliding;
	pad(6);
	_Vector2<EncryptedFloat> m_velocity;
	pad(8);
	int m_emotion_type;
	float m_emotion_time;
	pad(36);
	int m_user_id;
	pad(16);
	std::string m_country;
	pad(4);
	int m_flags;
	bool m_is_invis;
	bool m_is_mod;
	bool m_is_smod;
	pad(7);
	int m_bubble_state;
	pad(2);
	uint32_t m_state;
	pad(20);
	bool m_down_key_state;
	pad(3);
	uint32_t m_punch_effect;
	pad(52);
	EncryptedFloat m_gravity;
	EncryptedFloat m_acceleration;
	EncryptedFloat m_speed;

	_Vector2<int> GetTilePos() const
	{
		return
		{
			static_cast<int>(m_pos.x / 32.f),
			static_cast<int>(m_pos.y / 32.f)
		};
	}

	void SetPosAtTile(_Vector2<int> tile_pos)
	{
		m_pos.x = static_cast<float>(tile_pos.x) * 32.f;
		m_pos.y = static_cast<float>(tile_pos.y) * 32.f;
	}

	std::string GetName(bool raw = false) const
	{
		if (raw)
			return m_name;

		std::string name = m_name;
		name.erase(0, 2);
		name.erase(name.size() - 2, 2);

		return name;
	}
};
#pragma pack(pop)