/*
* - morphine - 
* 
* base growtopia cheat framework with the following features:
*     > sdk and module system
*     > base hooks
*     > clean codebase
*     > clean file system
*     > other various utilities
* 
* codestyle tutorial:
*     > global variables start with "g_" (eg. g_globals)
*     > classes start with "c_", excluding 3rd party classes from libraries, or game classes (eg. c_utils)
*     > structs, typedefs etc end with "_t" (eg. hook_t)
*     > class/struct members start with "m_" (eg. m_base_address)
*     > why am i writing this tutorial
*
* - made by trail & voidzero | licensed under the CC BY-NC-SA 4.0 -
*/

#pragma once
#include <chrono>
#include <thread>

#include "globals.hpp"
#include "config/config.hpp"
#include "hooks/hooks.hpp"
#include "hooks/render/renderer/renderer.hpp"
#include "utils/utils.hpp"
#include "utils/crash_handler.hpp"

// required by the mapper
extern "C" __declspec(dllexport) int init_fn(int code, WPARAM wParam, LPARAM lParam) { return CallNextHookEx(NULL, code, wParam, lParam); }

void on_inject(LPVOID param)
{
    g_utils->attach_console();

    g_globals->m_base_address = g_sdk->get_module().get_base_address().get<uintptr_t>();
    print(_("base address: 0x%llx"), g_globals->m_base_address);

    g_config->initialize();
    g_renderer->initialize();
    g_sdk->initialize();
    g_hooks->initialize();

    print(_("successfully initialized morphine"));

    /*
    * todo - @trailyy:
    * unloading the cheat is crashing
    * (something related to imgui and fonts)
    * ((((im too lazyt o fix)))
    */

    while (!g_globals->m_dll.m_unload)
        Sleep(1000);

    print(_("shutting down..."));

    g_hooks->shutdown();

    if (g_utils->is_console_attached())
        g_utils->detach_console();

    Sleep(750);

    FreeLibraryAndExitThread(g_globals->m_dll.m_self, 0);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID param)
{
    // start learning homie
    // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-disablethreadlibrarycalls
    DisableThreadLibraryCalls(module);

    if (reason == DLL_PROCESS_ATTACH)
    {
        // fuck createthread, all of my homies hate createthread
        _beginthreadex(nullptr, NULL, reinterpret_cast<unsigned(__cdecl*)(void*)>(on_inject), param, NULL, nullptr);

        // save the module for later
        g_globals->m_dll.m_self = module;
    }

    return TRUE;
}
