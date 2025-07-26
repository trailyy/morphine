#pragma once
#include "../hooks.hpp"

void __cdecl Hooked_ProcessTankUpdatePacket(GameLogicComponent* game_logic, GameUpdatePacket* packet)
{
	static auto orig = reinterpret_cast<decltype(g_sdk->m_process_tank_update_packet_fn)>(g_hooks->hkProcessTankUpdatePacket.m_orig);

	NetAvatar* local_player = game_logic->m_local_player;

	switch (packet->m_type)
	{
		case eGamePacketType::STATE:
		{
			if (packet->m_flags & eGamePacketFlags::MORPHINE)
			{
				NetAvatar* player = game_logic->m_net_object_manager.GetPlayerByNetID(packet->m_int1);

				if (std::find(g_globals->m_game.m_cheat_users.begin(), g_globals->m_game.m_cheat_users.end(), player)
					== g_globals->m_game.m_cheat_users.end())
				{
					g_globals->m_game.m_cheat_users.push_back(player);
					g_sdk->console_log(_("morphine user found: %s"), player->GetName().c_str());
				}
			}
		}
		break;

		case eGamePacketType::CALL_FUNCTION:
		{
			VariantList variant_list = VariantList(packet->get_extended_data());

			if (variant_list[1].GetString().length() <= 4096)
				variant_list.Print();
		}
		break;

		case eGamePacketType::SET_CHARACTER_STATE:
		{
			if (local_player && packet->m_int1 == local_player->m_net_id)
			{
				g_globals->m_game.m_state.m_flags = (ePlayerFlags)packet->m_flags;

				g_globals->m_game.m_state.m_water_speed = packet->m_float_data;
				g_globals->m_game.m_state.m_acceleration_speed = packet->m_vec1.x;
				g_globals->m_game.m_state.m_punch_strength = packet->m_vec1.y;
				g_globals->m_game.m_state.m_speed = packet->m_vec2.x;
				g_globals->m_game.m_state.m_gravity = packet->m_vec2.y;

				g_globals->m_game.m_state.m_punch_effect = packet->m_byte1;
				g_globals->m_game.m_state.m_build_range = packet->m_byte2 - 126; //128 
				g_globals->m_game.m_state.m_punch_range = packet->m_byte3 - 126;

				g_globals->m_game.m_state.m_pupil_color = packet->m_int2;
				g_globals->m_game.m_state.m_hair_color = packet->m_vec_int.x;
				g_globals->m_game.m_state.m_eye_color = packet->m_vec_int.y;
			}
		}
		break;
	}

	orig(game_logic, packet);
}