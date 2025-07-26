#pragma once

#include "utils.hpp"
#include "module.hpp"
#include "../lib/xorstr/xorstr.hpp"

c_utils* g_utils = new c_utils();

std::vector<uint8_t> string_to_bytes(std::string str)
{
    std::vector<std::uint8_t> bytes;

    char* start = const_cast<char*>(str.data());
    char* end = start + str.size();

    for (char* c = start; c < end; ++c)
        bytes.push_back(static_cast<std::uint8_t>(std::strtoul(c, &c, 16)));

    return bytes;
};

std::vector<uint8_t> pattern_to_bytes(const char* pat)
{
    std::vector<uint8_t> bytes{};

    auto start = (char*)pat;
    auto end = start + strlen(pat);

    for (auto curr = start; curr < end; ++curr)
    {
        if (*curr == '?')
        {
            ++curr;

            if (*curr == '?')
                ++curr;

            bytes.push_back(-1);

            continue;
        }

        bytes.push_back(strtoul(curr, &curr, 16));
    }

    return bytes;
};

void c_utils::attach_console()
{
    m_console.m_orig_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    m_console.m_orig_err_handle = GetStdHandle(STD_ERROR_HANDLE);
    m_console.m_orig_in_handle = GetStdHandle(STD_INPUT_HANDLE);

    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    m_console.m_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    m_console.m_err_handle = GetStdHandle(STD_ERROR_HANDLE);
    m_console.m_in_handle = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(m_console.m_out_handle, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleMode(m_console.m_in_handle, ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);

    freopen_s(&m_console.m_out_buffer, "nul", "w", stdout);

    auto outfd = _open_osfhandle((intptr_t)m_console.m_out_handle, _O_TEXT);
    auto outf = _fdopen(outfd, "w");

    if (outf)
    {
        m_console.m_file_handle = _fileno(stdout);
        _dup2(_fileno(outf), m_console.m_file_handle);
        setvbuf(stdout, NULL, _IONBF, 0);
    }

    print(_("console here bro"));
    m_console.m_attached = true;
}

void c_utils::detach_console()
{
    FreeConsole();

    if (m_console.m_file_handle != -1 && m_console.m_file_handle != 0)
        _dup2(_fileno(stdout), m_console.m_file_handle);

    if (m_console.m_orig_out_handle)
        SetStdHandle(STD_OUTPUT_HANDLE, m_console.m_orig_out_handle);
    if (m_console.m_orig_err_handle)
        SetStdHandle(STD_ERROR_HANDLE, m_console.m_orig_err_handle);
    if (m_console.m_orig_in_handle)
        SetStdHandle(STD_INPUT_HANDLE, m_console.m_orig_in_handle);

    m_console.m_attached = false;
}

bool c_utils::is_console_attached()
{
    return m_console.m_attached;
}

void c_utils::close_mutex()
{
    c_module ntdll_dll(_("ntdll.dll"));

    auto NtQuerySystemInformation = ntdll_dll.get_proc_address(_("NtQuerySystemInformation")).get<NtQuerySystemInformation_t>();
    auto NtQueryObject = ntdll_dll.get_proc_address(_("NtQueryObject")).get<NtQueryObject_t>();
    auto NtClose = ntdll_dll.get_proc_address(_("NtClose")).get<NtClose_t>();

    if (!NtQuerySystemInformation || !NtQueryObject || !NtClose)
    {
        print(_("[ mutex ] could not resolve one or more functions"));
        return;
    }

    std::vector<BYTE> buffer;
    ULONG buffer_size = 1 << 20;

    NTSTATUS status = { };

    do
    {
        buffer.resize(buffer_size);
        status = NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)16, buffer.data(), buffer_size, &buffer_size);
    } while (status == STATUS_INFO_LENGTH_MISMATCH);

    if (!NT_SUCCESS(status))
    {
        print(_("[ mutex ] could not query handle information"));
        return;
    }

    auto info = reinterpret_cast<SYSTEM_HANDLE_INFORMATION*>(buffer.data());

    int total_found = 0;

    for (ULONG i = 0; i < info->NumberOfHandles; ++i)
    {
        const auto& entry = info->Handles[i];

        if (entry.UniqueProcessId != GetCurrentProcessId())
            continue;

        HANDLE handle = reinterpret_cast<HANDLE>(UINT_PTR(entry.HandleValue));

        ULONG type_len = 0;
        status = NtQueryObject(handle, (OBJECT_INFORMATION_CLASS)2, nullptr, 0, &type_len);

        if (status != STATUS_INFO_LENGTH_MISMATCH)
            continue;

        std::vector<BYTE> type_buffer(type_len);
        if (!NT_SUCCESS(NtQueryObject(handle, (OBJECT_INFORMATION_CLASS)2, type_buffer.data(), type_len, &type_len)))
            continue;

        auto object_type_information = (OBJECT_TYPE_INFORMATION*)(type_buffer.data());
        std::wstring type(object_type_information->TypeName.Buffer, object_type_information->TypeName.Length / sizeof(WCHAR));

        if (type != L"Mutant")
            continue;

        ULONG name_len = 0;
        NtQueryObject(handle, (OBJECT_INFORMATION_CLASS)1, nullptr, 0, &name_len);

        std::vector<BYTE> name_buffer(name_len);
        if (!NT_SUCCESS(NtQueryObject(handle, (OBJECT_INFORMATION_CLASS)1, name_buffer.data(), name_len, nullptr)))
            continue;

        auto name_information = (OBJECT_NAME_INFORMATION*)(name_buffer.data());
        std::wstring name(name_information->Name.Buffer, name_information->Name.Length / sizeof(WCHAR));

        if (!name.ends_with(L"BaseNamedObjects\\Growtopia"))
            continue;

        total_found++;
        NtClose(handle);

        print(_("[ mutex ] closed mutex %s (%d)"), std::string(name.begin(), name.end()).c_str(), total_found);
    }
}

std::string c_utils::get_packet_type(eGamePacketType type)
{
    static std::string types[]
    {
        _("PACKET_STATE"),
        _("PACKET_CALL_FUNCTION"),
        _("PACKET_UPDATE_STATUS"),
        _("PACKET_TILE_CHANGE_REQUEST"),
        _("PACKET_SEND_MAP_DATA"),
        _("PACKET_SEND_TILE_UPDATE_DATA"),
        _("PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE"),
        _("PACKET_TILE_ACTIVATE_REQUEST"),
        _("PACKET_TILE_APPLY_DAMAGE"),
        _("PACKET_SEND_INVENTORY_STATE"),
        _("PACKET_ITEM_ACTIVATE_REQUEST"),
        _("PACKET_ITEM_ACTIVATE_OBJECT_REQUEST"),
        _("PACKET_SEND_TILE_TREE_STATE"),
        _("PACKET_MODIFY_ITEM_INVENTORY"),
        _("PACKET_ITEM_CHANGE_OBJECT"),
        _("PACKET_SEND_LOCK"),
        _("PACKET_SEND_ITEM_DATABASE_DATA"),
        _("PACKET_SEND_PARTICLE_EFFECT"),
        _("PACKET_SET_ICON_STATE"),
        _("PACKET_ITEM_EFFECT"),
        _("PACKET_SET_CHARACTER_STATE"),
        _("PACKET_PING_REPLY"),
        _("PACKET_PING_REQUEST"),
        _("PACKET_GOT_PUNCHED"),
        _("PACKET_APP_CHECK_RESPONSE"),
        _("PACKET_APP_INTEGRITY_FAIL"),
        _("PACKET_DISCONNECT"),
        _("PACKET_BATTLE_JOIN"),
        _("PACKET_BATTLE_EVENT"),
        _("PACKET_USE_DOOR"),
        _("PACKET_SEND_PARENTAL"),
        _("PACKET_GONE_FISHIN"),
        _("PACKET_STEAM"),
        _("PACKET_PET_BATTLE"),
        _("PACKET_NPC"),
        _("PACKET_SPECIAL"),
        _("PACKET_SEND_PARTICLE_EFFECT_V2"),
        _("PACKET_ACTIVE_ARROW_TO_ITEM"),
        _("PACKET_SELECT_TILE_INDEX"),
        _("PACKET_SEND_PLAYER_TRIBUTE_DATA"),
        _("PACKET_PVE_UNK1"),
        _("PACKET_PVE_UNK2"),
        _("PACKET_PVE_UNK3"),
        _("PACKET_PVE_UNK4"),
        _("PACKET_PVE_UNK5"),
        _("PACKET_SET_EXTRA_MODS"),
        _("PACKET_ON_STEP_ON_TILE_MOD")
    };

    constexpr auto size = sizeof(types) / sizeof(std::string) - 1;

    if (type > eGamePacketType::ON_STEP_ON_TILE_MOD || (int)type > size)
        type = eGamePacketType::ON_STEP_ON_TILE_MOD;

    return types[(int)type];
}

std::string c_utils::get_packet_flags(uint32_t flags)
{
    std::map<std::string, uint32_t> flags_map =
    {
        { _("UNK_0x1"), 0x1 },
        { _("FLYING"), 0x2 },
        { _("RESET_VISUAL_STATE"), 0x4 },
        { _("EXTENDED"), 0x8 },
        { _("FACING_LEFT"), 0x10 },
        { _("ON_SOLID"), 0x20 },
        { _("ON_FIRE_DAMAGE"), 0x40 },
        { _("ON_JUMP"), 0x80 },
        { _("ON_KILLED"), 0x100 },
        { _("ON_PUNCHED"), 0x200 },
        { _("ON_PLACED"), 0x400 },
        { _("ON_TILE_ACTION"), 0x800 },
        { _("ON_GOT_PUNCHED"), 0x1000 },
        { _("ON_RESPAWNED"), 0x2000 },
        { _("ON_COLLECT_OBJECT"), 0x4000 },
        { _("ON_TRAMPOLINE"), 0x8000 },
        { _("ON_DAMAGE"), 0x10000 },
        { _("ON_SLIDE"), 0x20000 },
        { _("ON_PARASOL"), 0x40000 },
        { _("UNK_0x80000"), 0x80000 },
        { _("ON_SWIM"), 0x100000 },
        { _("ON_WALL_HANG"), 0x200000 },
        { _("ON_PUNCH_START"), 0x400000 },
        { _("ON_PUNCH_END"), 0x800000 },
        { _("UNK_0x1000000"), 0x1000000 },
        { _("UNK_0x2000000"), 0x2000000 },
        { _("UNK_0x4000000"), 0x4000000 },
        { _("UNK_0x8000000"), 0x8000000 },
        { _("ON_ACID_DAMAGE"), 0x10000000 },
        { _("MORPHINE"), 0x40000000 }
    };

    bool first = true;
    std::string ret = _("");

    for (const auto& flag : flags_map)
    {
        if ((flags & flag.second) == flag.second)
        {
            if (!first)
                ret += _(", ");

            ret += flag.first;
            first = false;
        }
    }

    return ret.empty() ? _("none") : ret;
}

template <typename T>
bool c_utils::write_memory_ex(void* address, T value)
{
    DWORD old = 0;

    if (!VirtualProtect(address, sizeof(T), PAGE_EXECUTE_READWRITE, &old))
        return false;

    memcpy(address, (void*)&value, sizeof(value));

    return VirtualProtect(address, sizeof(T), old, &old);
}

bool c_utils::write_memory(void* address, std::string bytes_s)
{
    DWORD old = 0;

    std::vector<std::uint8_t> bytes = string_to_bytes(bytes_s);

    if (!VirtualProtect(address, bytes.size(), PAGE_EXECUTE_READWRITE, &old))
        return false;

    memcpy(address, bytes.data(), bytes.size());

    return VirtualProtect(address, bytes.size(), old, &old);
}

template <typename T>
T c_utils::read_memory(uintptr_t address)
{
    T ret = *(T*)(address);
    return ret;
}