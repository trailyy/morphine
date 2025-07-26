#pragma once
#include <cstdint>
#include "../../lib/xorstr/xorstr.hpp"
#include "../proton/Vector.hpp"
#include "../enums.hpp"

#pragma pack(push, 1)
struct GameUpdatePacket
{
	eGamePacketType m_type;
	uint8_t m_byte1;
	uint8_t m_byte2;
	uint8_t m_byte3;
	int m_int1;
	int m_int2;
	int m_flags;
	float m_float_data;
	int m_int_data;
	Vector2 m_vec1;
	Vector2 m_vec2;
	float m_float;
	_Vector2<int> m_vec_int;
	uint32_t m_extended_data_size;

	inline uint8_t* get_extended_data()
	{
		return reinterpret_cast<uint8_t*>(this) + sizeof(GameUpdatePacket);
	}

    void debug_print()
    {
		print(_("---- gameupdatepacket ----"));
        print(_("m_type: %d"), m_type);
        print(_("m_byte1: %d"), m_byte1);
        print(_("m_byte2: %d"), m_byte2);
        print(_("m_byte3: %d"), m_byte3);
        print(_("m_int1: %d"), m_int1);
        print(_("m_int2: %d"), m_int2);
        print(_("m_flags: %d (%s)"), m_flags, g_utils->get_packet_flags(m_flags).c_str());
        print(_("m_float_data: %.2f"), m_float_data);
        print(_("m_int_data: %d"), m_int_data);
        print(_("m_vec1.x: %.2f | m_vec1.y: %.2f"), m_vec1.x, m_vec1.y);
		print(_("m_vec2.x: %.2f | m_vec2.y: %.2f"), m_vec2.x, m_vec2.y);
        print(_("m_float: %.2f"), m_float);
        print(_("m_vec_int.x: %d | vec_int.y: %d"), m_vec_int.x, m_vec_int.y);
		print(_("m_extended_data_size: %d"), m_extended_data_size);
		print(_("--------------------------"));
    }
};
#pragma pack(pop)

static_assert(sizeof(GameUpdatePacket) == 56);