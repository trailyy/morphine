#pragma once
#include <cstdint>
#include "TileExtra.hpp"
#include "../proton/Vector.hpp"
#include "../enums.hpp"

#pragma pack(push, 1)
class Tile
{
public:
	Vector2 GetWorldPos() const
	{
		return
		{
			static_cast<float>(m_pos.x) * 32.f,
			static_cast<float>(m_pos.y) * 32.f
		};
	}

	pad(4);
	uint16_t m_foreground; // 0x4
	uint16_t m_background; // 0x6
	eTileFlags m_flags; // 0x8
	_Vector2<uint8_t> m_pos; // 0xa
	pad(12);
	Vector4 m_collision_rect; // 0x18
	TileExtra* m_extra; // 0x28
	pad(112);
};
#pragma pack(pop)

static_assert(sizeof(Tile) == 0xA0);