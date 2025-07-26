/*
* setwindowshookex injector
*
* made by voidzero & trail
*/

#pragma once
#pragma warning(disable : 6387)

#include <windows.h>
#include <iostream>

// at least one must be known/valid
#define CLASS_NAME "AppClass"
#define WINDOW_NAME "Growtopia" // cheat adds " | morphine" to the title to support multiple instances

#define DLL_NAME "morphine.dll"

typedef int(__cdecl* cheat_initialize_t)();
typedef NTSTATUS(_stdcall* _NtSetInformationProcess)(HANDLE, PROCESS_INFORMATION_CLASS, PVOID, ULONG);

BOOL set_mitigation_policy(DWORD pid, PROCESS_MITIGATION_POLICY mitigation_policy, PVOID buffer, SIZE_T length)
{
	BOOL result = FALSE;

	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_SET_INFORMATION, FALSE, pid);

	if (proc != NULL)
	{
		_NtSetInformationProcess NtSetInformationProcess = reinterpret_cast<_NtSetInformationProcess>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtSetInformationProcess"));

		uintptr_t policy = *static_cast<DWORD*>(buffer);
		policy = policy << 32;
		policy += static_cast<DWORD>(mitigation_policy);

		NTSTATUS ret = NtSetInformationProcess(proc, static_cast<PROCESS_INFORMATION_CLASS>(0x34), &policy, sizeof(policy));
		if (ret == 0) result = TRUE;

		CloseHandle(proc);
	}

	return result;
}

BOOL force_debug_permission()
{
	BOOL result = false;

	HANDLE token;
	result = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

	if (!result) return FALSE;

	LUID luid;
	result = LookupPrivilegeValueA(NULL, SE_DEBUG_NAME, &luid);

	if (!result)
	{
		CloseHandle(token);
		return FALSE;
	}

	TOKEN_PRIVILEGES token_privileges;
	token_privileges.PrivilegeCount = 1;
	token_privileges.Privileges[0].Luid = luid;
	token_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	result = AdjustTokenPrivileges(token, FALSE, &token_privileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	if (!result)
	{
		CloseHandle(token);
		return FALSE;
	}

	printf("[ mapper ] forced debug permission\n");

	CloseHandle(token);
	return TRUE;
}

int main()
{
	if (!force_debug_permission())
	{
		printf("[ mapper ] failed to get debug permission\n");
		system("pause");

		return 1;
	}

	printf("[ mapper ] waiting for game...\n");

	HWND hwnd;
	do
	{
		hwnd = FindWindowA(CLASS_NAME, WINDOW_NAME);
		Sleep(250);
	} while (hwnd == NULL);

	printf("[ mapper ] got hwnd @ 0x%llx\n", (uintptr_t)hwnd);

	DWORD process_id;
	DWORD thread_id = GetWindowThreadProcessId(hwnd, &process_id);

	PROCESS_MITIGATION_DYNAMIC_CODE_POLICY policy = {};
	policy.ProhibitDynamicCode = FALSE;

	if (!set_mitigation_policy(process_id, ProcessDynamicCodePolicy, &policy, sizeof(policy)))
	{
		printf("[ mapper ] failed to set policy\n");
		system("pause");

		return 2;
	}

	HINSTANCE cheat_dll = LoadLibraryExA(DLL_NAME, NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (!cheat_dll)
	{
		printf("[ mapper ] failed to load module\n");
		system("pause");

		return 3;
	}

	cheat_initialize_t init_fn = reinterpret_cast<cheat_initialize_t>(GetProcAddress(cheat_dll, "init_fn"));
	if (!init_fn)
	{
		printf("[ mapper ] failed to find init_fn\n");
		system("pause");

		return 4;
	}

	printf("[ mapper ] init_fn @ 0x%llx\n", reinterpret_cast<uintptr_t>(init_fn));

	HHOOK handle = SetWindowsHookExA(WH_GETMESSAGE, reinterpret_cast<HOOKPROC>(init_fn), cheat_dll, thread_id);

	if (!PostThreadMessageA(thread_id, WM_NULL, NULL, NULL))
		printf("[ mapper ] could not trigger hook (%d)\n", GetLastError());

	printf("[ mapper ] successfully injected morphine, press any key to unhook\n");
	system("pause");

	UnhookWindowsHookEx(handle);

	return 0;
}