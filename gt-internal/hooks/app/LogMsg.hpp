#pragma once
#include "../hooks.hpp"
#include "../../lib/xorstr/xorstr.hpp"

// found in my old project, so why not salvage?
using log_msg_t = void(__cdecl*)(const char*, ...);
int __cdecl Hooked_LogMsg(const char* msg, ...)
{
    static auto orig = reinterpret_cast<decltype(g_sdk->m_log_msg_fn)>(g_hooks->hkLogMsg.m_orig);

    char buf[0x1000];
    int len = -1;

    va_list va;
    va_start(va, msg);

    len = _vsnprintf_s(buf, sizeof(buf), msg, va);
    if (len < 0)
    {
        va_end(va);
        return 0;
    }

    va_end(va);

    if (len < 100 && len > 2)
    {
        const char* logs = buf;
        log(_("%s"), logs);
    }

    orig(buf);
}