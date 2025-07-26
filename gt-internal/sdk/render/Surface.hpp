#pragma once
#include <string>
#include "../proton/Boost.hpp"
#include "../proton/Vector.hpp"
#include "../enums.hpp"

#pragma pack(push, 1)
class Surface
{
public:
	void* m_vtable;
	BoostTrackableSignal m_trackable_signal;
	uint16_t m_texture_id;
	pad(2);
	Vector2 m_texture_size;
	Vector2 m_original_size;
	bool m_alpha;
	eTextureType m_texture_type;
	eBlendingMode m_blending_mode;
	int m_mip_map_count;
	uint32_t m_frame_buffer;
	int m_memory_used;
	std::string m_texture_loaded;
	bool m_create_mip_maps_if_needed;
	eTextureCreationType m_texture_creation_method;
	bool m_smoothing;
	bool m_add_base_path;
};
#pragma pack(pop)
