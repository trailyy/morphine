#include "../hooks.hpp"

bool __cdecl Hooked_CanMessageT4(NetAvatar* net_avatar)
{
	static auto orig = reinterpret_cast<decltype(g_sdk->m_can_message_t4_fn)>(g_hooks->hkCanMessageT4.m_orig);

	return false;
}