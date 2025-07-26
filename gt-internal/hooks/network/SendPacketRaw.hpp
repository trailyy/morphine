#pragma once
#include "../hooks.hpp"
#include "../../config/config.hpp"

void __cdecl Hooked_SendPacketRaw(int type, GameUpdatePacket* packet, int size, void* packet_sender, void* enet_peer, int flag)
{
	static auto orig = reinterpret_cast<decltype(g_sdk->m_send_packet_raw_fn)>(g_hooks->hkSendPacketRaw.m_orig);

	switch (packet->m_type)
	{
		case eGamePacketType::APP_INTEGRITY_FAIL:
			return;
			break;

		case eGamePacketType::STATE:
		{
			packet->m_flags |= eGamePacketFlags::MORPHINE;
		}
		break;
		
		case eGamePacketType::PING_REPLY:
		{
			packet->m_byte2 = 0;
			packet->m_byte3 = 0;

			packet->m_vec1.x = g_globals->m_game.m_state.m_punch_range * 32.f;
			packet->m_vec1.y = g_globals->m_game.m_state.m_build_range * 32.f;

			if (g_sdk->get_game_logic()->m_local_player)
			{
				packet->m_int1 = static_cast<int>(g_globals->m_game.m_state.m_flags);
				packet->m_vec2.x = g_globals->m_game.m_state.m_gravity;
				packet->m_vec2.y = g_globals->m_game.m_state.m_speed;
			}
			
		}
		break;
	}

	print(_("sending raw packet | %d : %s"), packet->m_type, g_utils->get_packet_type(packet->m_type).c_str());

	if (g_config->m_misc.m_debug_packets)
		packet->debug_print();

	orig(type, packet, size, packet_sender, enet_peer, flag);
}