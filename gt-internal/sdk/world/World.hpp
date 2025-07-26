#pragma once
#include <cstdint>
#include <string>

#include "WorldTileMap.hpp"
#include "WorldObjectMap.hpp"

#pragma pack(push, 1)
class World
{
public:
	void* m_vtable; // 0x0
	pad(2);
	uint16_t m_version; // 0xA
	pad(124);
	WorldTileMap m_tile_map;
	WorldObjectMap m_object_map;
	pad(8);
	std::string m_name;
	pad(4);
	int m_weather;
	int m_weather_banners;
};
#pragma pack(pop)