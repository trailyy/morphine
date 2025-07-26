#pragma once
#include <map>
#include "../player/NetAvatar.hpp"

#pragma pack(push, 1)
class NetObjectManager
{
public:
	void* vtable;
	std::map<int, NetAvatar*> m_players;
	pad(56);

	NetAvatar* GetPlayerByNetID(int i)
	{
		if (m_players.empty())
			return NULL;

		return m_players[i] ? m_players[i] : NULL;
	}

	NetAvatar* operator[](int i)
	{
		return GetPlayerByNetID(i);
	}
};
#pragma pack(pop)