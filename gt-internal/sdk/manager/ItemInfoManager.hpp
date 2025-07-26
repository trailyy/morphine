#pragma once
#include <cstdint>
#include <vector>
#include "../enums.hpp"

#pragma pack(push, 1)
class ItemInfo
{
public:
	uint32_t m_id;
	eItemType m_type;
	eMaterialType m_material;
	pad(4);
	std::string m_name;
	uint32_t m_texture_hash;
	pad(4);
	std::string m_texture_path;
	eVisualEffectType m_visual_effect;
	int m_cook_time;
	_Vector2<uint8_t> m_texture_pos;
	pad(2);
	eTileStorageType m_storage;
	uint32_t m_compute_value;
	eTileCollisionType m_collision;
	uint16_t m_hits_to_destroy;
	eItemFlags m_flags;
	pad(68);
	uint32_t m_heal_time;
	eBodyPart m_body_part;
	char m_seed_base;
	char m_seed_overlay;
	char m_tree_base;
	char m_tree_leaves;
	uint32_t m_seed_color;
	uint32_t m_seed_overlay_color;
	pad(4);
	uint32_t m_grow_time;
	pad(82);
	uint16_t m_rarity;
	char m_max_can_hold;
	pad(7);
	std::string m_extra_file;
	uint32_t m_extra_file_hash;
	uint32_t m_anim_ms;
	pad(40);
	uint32_t m_variant_item;
	pad(4);
	std::string m_pet_name;
	std::string m_pet_prefix;
	std::string m_pet_suffix;
	std::string m_pet_ability;
	pad(32);
	std::string m_unk_strs[8];
	pad(4);
	eItemFxFlags m_fx_flags;
	std::string m_extra_options;
	std::string m_extra_options2;
	std::string m_texture2_path;
	pad(246);

	struct extra_slots_t
	{
		uint32_t m_body_part1;
		uint32_t m_body_part2;
		uint32_t m_body_part3;
		uint32_t m_body_part4;
		uint32_t m_body_part5;
		uint32_t m_body_part6;
		uint32_t m_body_part7;
		uint32_t m_body_part8;
		uint32_t m_body_part9;
	} m_extra_slots;

	uint32_t m_extra_slot_count;
	std::string m_punch_parameters;
	pad(704);
};

class ItemInfoManager
{
public:
	void* m_vtable;
	std::vector<ItemInfo> m_items;

	ItemInfo* GetItem(uint32_t item_id)
	{
		if (m_items.empty())
			return nullptr;

		if (item_id < m_items.size())
			return &m_items.at(item_id);

		return nullptr;
	}

	std::string GetItemName(uint32_t item_id)
	{
		if (ItemInfo* item = GetItem(item_id); item != nullptr)
			return item->m_name;

		return "<invalid>";
	}
};
#pragma pack(pop)