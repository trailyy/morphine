#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "../enums.hpp"

#pragma pack(push, 1)
class TileExtra
{
public:
	void* m_vtable;
	eTileExtraType m_type;
	pad(3);

	union
	{
		int m_item_price;
		uint32_t m_lock_owner;
	};

	pad(32);
	std::string m_label;
	pad(32);

	union
	{
		uint32_t m_item_id;
		uint32_t m_last_update;
	};

	uint32_t m_growth;
	char m_fruit_count;
	pad(31);
	std::vector<uint32_t> m_access_list;
};
#pragma pack(pop)