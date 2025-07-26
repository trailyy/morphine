#include "../hooks.hpp"

BOOL WINAPI Hooked_CheckRemoteDebuggerPresent(HANDLE handle, PBOOL out)
{
	print("spoofing CheckRemoteDebuggerPresent (h: %d)", handle);
	*out = false;

	return TRUE;
}