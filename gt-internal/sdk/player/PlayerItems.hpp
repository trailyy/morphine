#pragma once
#include <cstdint>
#include <list>

#pragma pack(push, 1)
struct InventoryItem
{
	uint16_t m_item_id;
	uint8_t m_count;
	uint8_t m_flags;
};

class PlayerItems
{
public:
	void* m_vtable;
	uint16_t m_clothes[12];
	std::vector<InventoryItem> m_items; // 0x20
	uint16_t m_quick_slots[4];
	uint32_t m_backpack_size; // 0x38
	pad(4);
	std::vector<InventoryItem> m_filtered_items; // 0x40
	pad(8);
	uint16_t m_selected_item; // 0x58
	pad(6);
	int m_gems; // 0x60
	pad(4);
	int m_gems_collected; // 0x70
	pad(68);
};
#pragma pack(pop)