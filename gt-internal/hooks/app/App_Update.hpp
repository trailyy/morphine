#pragma once
#include "../hooks.hpp"

#include "../../config/config.hpp"

void __cdecl Hooked_Update(App* app)
{
	static auto orig = reinterpret_cast<decltype(g_sdk->m_app_update_fn)>(g_hooks->hkUpdate.m_orig);

	static bool joined_world = false;

	if (app->m_game_logic_component)
	{
		NetAvatar* local_player = app->m_game_logic_component->m_local_player;
		if (local_player)
		{
			if (!joined_world)
			{
				g_globals->m_game.m_cheat_users.clear();

				GameUpdatePacket packet = { };
				packet.m_type = eGamePacketType::STATE;
				packet.m_flags = eGamePacketFlags::MORPHINE;
				g_sdk->send_packet_raw(&packet);

				local_player->m_is_mod = g_config->m_cheats.m_mod_zoom;
				
				joined_world = true;
			}

			for (auto e : g_globals->m_game.m_cheat_users)
			{
				if (e == nullptr)
					continue;

				std::string name = e->GetName(true);

				if (name.find(_("[ morphine ]")) == std::string::npos)
				{
					name.erase(name.size() - 2, 2);
					e->m_name = name + _(" [ morphine ]``");
				}
			}
		}
		else
			joined_world = false;
	}

	static float hue = 0.0f;
	
	hue += 0.01f;
	if (hue >= 360.0f) hue -= 360.0f;
	
	Entity* morphine_text = g_sdk->get_entity_root()->GetEntityByName(_("morphine_watermark"));

	if (morphine_text)
		morphine_text->GetVariant(_("color"))->Set(g_utils->hsv_to_rgb(hue, 1.0f, 1.0f));

	orig(app);
}