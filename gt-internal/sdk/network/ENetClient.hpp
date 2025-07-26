#pragma once
#include <cstdint>
#include <string>
#include "../proton/Boost.hpp"

#pragma pack(push, 1)
class ENetClient
{
public:
	void* m_vtable;
	BoostSignal m_sig_unk1;
	BoostSignal m_sig_unk2;
	BoostSignal m_sig_unk3;
	BoostSignal m_sig_unk4;
	void* m_host;
	void* m_peer;
	uint32_t m_timed_out_timestamp;
	pad(12);
	std::string m_server_ip;
	uint32_t m_server_port;
	pad(4);
	int m_token;
	int m_user;
	std::string m_door_id;
	int m_lmode;
	pad(4);
	std::string m_uuid_token;
};
#pragma pack(pop)