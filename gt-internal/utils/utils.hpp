#pragma once
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <random>
#include <fcntl.h>
#include <io.h>
#include <vector>
#include <aclapi.h>
#include <map>

#include <winternl.h>
#include <ntstatus.h>

#include "../sdk/enums.hpp"

#define print(...) g_utils->fancy_log(__VA_ARGS__)
#define log(...) g_utils->game_log(__VA_ARGS__)

struct SYSTEM_HANDLE_TABLE_ENTRY_INFO
{
    USHORT  UniqueProcessId;
    USHORT  CreatorBackTraceIndex;
    UCHAR   ObjectTypeIndex;
    UCHAR   HandleAttributes;
    USHORT  HandleValue;
    PVOID   Object;
    ULONG   GrantedAccess;
};

struct SYSTEM_HANDLE_INFORMATION
{
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
};

struct OBJECT_NAME_INFORMATION
{
    UNICODE_STRING Name;
};

struct OBJECT_TYPE_INFORMATION
{
    UNICODE_STRING TypeName;
};

typedef NTSTATUS(WINAPI* NtQuerySystemInformation_t)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

typedef NTSTATUS(WINAPI* NtDuplicateObject_t)(
    HANDLE SourceProcessHandle,
    HANDLE SourceHandle,
    HANDLE TargetProcessHandle,
    PHANDLE TargetHandle,
    ACCESS_MASK DesiredAccess,
    ULONG Attributes,
    ULONG Options
);

typedef NTSTATUS(WINAPI* NtQueryObject_t)(
    HANDLE Handle,
    OBJECT_INFORMATION_CLASS ObjectInformationClass,
    PVOID ObjectInformation,
    ULONG ObjectInformationLength,
    PULONG ReturnLength
);

typedef NTSTATUS(WINAPI* NtClose_t)(HANDLE Handle);

#ifndef ObjectNameInformation
#define ObjectNameInformation ((OBJECT_INFORMATION_CLASS)1)
#endif

class c_utils
{
private:
    uint32_t m_seed;

    struct console_data_t
    {
        HANDLE m_orig_out_handle;
        HANDLE m_orig_err_handle;
        HANDLE m_orig_in_handle;

        HANDLE m_out_handle;
        HANDLE m_err_handle;
        HANDLE m_in_handle;

        FILE* m_out_buffer = nullptr;
        int m_file_handle;

        bool m_attached;
    } m_console;
public:
    void attach_console();
    void detach_console();
    bool is_console_attached();
    void close_mutex();

    void fancy_log(const char* message, ...)
    {
        va_list va;
        va_start(va, message);  
        printf("\033[32m[ sdk ]\033[0m ");
        vprintf(message, va);
        printf("\n");

        va_end(va);
    }

    void game_log(const char* message, ...)
    {
        va_list va;
        va_start(va, message);  
        printf("[ log ] " );
        vprintf(message, va);
        printf("\n");

        va_end(va);
    }

    std::string format(const char* format, ...)
    {
        char buffer[1024];

        va_list va;
        va_start(va, format);

        vsnprintf(buffer, sizeof(buffer), format, va);

        va_end(va);

        return std::string(buffer);
    }

    std::string to_multibyte(wchar_t* buffer)
    {
        auto wstring = std::wstring(buffer);

        if (wstring.empty())
            return "";

        return std::string(wstring.begin(), wstring.end());
    }

    uint32_t hsv_to_rgb(float h, float s, float v)
    {
        float c = v * s;
        float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
        float m = v - c;
        float r1, g1, b1;
        if (h < 60) { r1 = c; g1 = x; b1 = 0; }
        else if (h < 120) { r1 = x; g1 = c; b1 = 0; }
        else if (h < 180) { r1 = 0; g1 = c; b1 = x; }
        else if (h < 240) { r1 = 0; g1 = x; b1 = c; }
        else if (h < 300) { r1 = x; g1 = 0; b1 = c; }
        else { r1 = c; g1 = 0; b1 = x; }

        uint8_t r = uint8_t((r1 + m) * 255);
        uint8_t g = uint8_t((g1 + m) * 255);
        uint8_t b = uint8_t((b1 + m) * 255);
        uint8_t a = 0xFF;

        return (b << 24) | (g << 16) | (r << 8) | a;
    }

    std::string get_packet_type(eGamePacketType type);
    std::string get_packet_flags(uint32_t flags);
    
    uint32_t lcg()
    {
        m_seed = 1664525 * m_seed + 1013904223;
        return m_seed;
    }

    int random(int min, int max)
    {
        return min + (lcg() % (max - min + 1));
    }

    template <typename T, typename... Args>
    T call_vfunc(uint32_t idx, void* thisptr, Args... args)
    {
        using function_t = T(__thiscall***)(void*, Args...);
        auto function = (*static_cast<function_t>(thisptr))[idx];

        return function(thisptr, args...);
    }

    template <typename T = void*>
    T get_vfunc(uint32_t idx, void* thisptr)
    {
        void** vtable = *static_cast<void***>(thisptr);
        return reinterpret_cast<T>(vtable[idx]);
    }

    template <typename T>
    bool write_memory_ex(void* address, T value);
    bool write_memory(void* address, std::string bytes);

    template <typename T>
    T read_memory(uintptr_t address);
};

extern c_utils* g_utils;