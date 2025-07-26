#include "../hooks.hpp"

BOOL WINAPI Hooked_IsDebuggerPresent()
{
	print("spoofing IsDebuggerPresent");

	return FALSE;
}