#pragma once
#include "../hooks.hpp"
#include "../../sdk/proton/TextScanner.hpp"

void __cdecl Hooked_SendPacket(int type, std::string& packet, void* peer)
{
	static auto orig = reinterpret_cast<decltype(g_sdk->m_send_packet_fn)>(g_hooks->hkSendPacket.m_orig);

	switch (type)
	{
		case eNetMessageType::NET_MESSAGE_GENERIC_TEXT:
		{
			TextScanner parsed_packet(packet);

			if (parsed_packet.Has("game_version"))
			{
				parsed_packet.Set("country", "cn");
				packet = parsed_packet.Dump();
			}
		}
		break;
	}

	print(_("sending text packet | %d : %s"), type, packet.c_str());

	orig(type, packet, peer);
}