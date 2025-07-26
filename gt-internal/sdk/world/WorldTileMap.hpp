#pragma once
#include <vector>
#include <cstdint>
#include "Tile.hpp"
#include "../proton/Vector.hpp"

class World;

#pragma pack(push, 1)
class WorldTileMap
{
public:
	void* m_vtable;
	_Vector2<int> m_size;
	pad(8);
	std::vector<Tile> m_tile_map;
	World* m_world;

	Tile* GetTile(std::int32_t x, std::int32_t y) noexcept
	{
		if (x >= 0 && y >= 0 && m_size.x > x && m_size.y > y)
			return &m_tile_map[static_cast<std::size_t>(y * m_size.x + x)];

		return nullptr;
	}

	Tile* GetTile(const _Vector2<int>& pos) noexcept
	{
		return GetTile(pos.x, pos.y);
	}

	Tile* GetTileAtWorldPos(const _Vector2<int>& pos) noexcept
	{
		return GetTile
		(
			static_cast<std::int32_t>(pos.x / 32.f),
			static_cast<std::int32_t>(pos.y / 32.f)
		);
	}
};
#pragma pack(pop)