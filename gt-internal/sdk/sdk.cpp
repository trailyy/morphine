#pragma once
#include "../globals.hpp"

c_sdk* g_sdk = new c_sdk();

c_module c_sdk::get_module() const
{
	return m_game_module;
}

void c_sdk::initialize()
{
	/*
	* tl;dr:
	* themida/vmprotect hook NtProtectVirtualMemory
	* and various other functions such as NtReadVM, NtWriteVM (etc.)
	* for their own usage and in order to prevent 3rd party software
	* from calling those functions and their highlevel wrappers
	* (VirtualProtect, VirtualQuery, WPM, RPM, etc.)
	* 
	* you can check the entire list of hooked functions by yourself
	* with for example cheat engine (scan for patches->ntdll, kernelbase, etc.)
	* 
	* here we restore the crucial one (in our case)
	*/

	// get handle to ntdll.dll
	c_module ntdll(_("ntdll.dll"));

	void* nt_protect_virtual_memory_fn = ntdll.get_proc_address(_("NtProtectVirtualMemory")).get();

	/*
	*       
	*       the hook 
	* jmp 7FFFxxxxxxxx -> mov r10,rcx (4c 8b d1)
	*					  mov eax,50  (b8 50)
	* 
	* simple as that bro how crazy is that
	*/

	g_utils->write_memory(nt_protect_virtual_memory_fn, _("4C 8B D1 B8 50"));
	
	print(_("restored ntdll.dll!NtProtectVirtualMemory"));

	// mutex bullshit fix (open many growtopia gabb method 2017 no save.dat stealer working 2025 bots enet proxy support socks5)
	g_utils->write_memory(m_game_module.find_pattern(_("74 ? 48 8B C8 E8 ? ? ? ? CF"), _("MutexPatch")).get(), _("90 90"));
	g_utils->close_mutex();

	// so mapper doesnt get confused
	if (!SetWindowTextA(g_globals->m_dll.m_window, _("Growtopia | morphine")))
		print(_("could not set window title (%d)"), GetLastError());

	/* 
	* todo - @trailyy:
	* make sure patterns are up
	* to date with each version
	* and make the patterns more 
	* unique
	*/

	// "get" type functions for core classes
	m_get_app_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 8B 4F 5C"), _("GetApp"))
		.relative()
		.get<decltype(m_get_app_fn)>();
	m_get_base_app_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 41 8B 46 58"), _("GetBaseApp"))
		.relative()
		.get<decltype(m_get_base_app_fn)>();
	m_get_game_logic_fn = m_game_module.find_pattern(_("48 83 EC ? E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 83 C4"), _("GetGameLogic"))
		.get<decltype(m_get_game_logic_fn)>();
	m_get_entity_root_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 48 8B C8 FF 57 20"), _("GetEntityRoot"))
		.relative()
		.get<decltype(m_get_entity_root_fn)>();
	m_get_enet_client_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 89 74 24 28"), _("GetClient"))
		.relative()
		.get<decltype(m_get_enet_client_fn)>();
	m_get_item_info_manager_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 48 69 D3 ? ? ? ? B9"), _("GetItemInfoManager"))
		.relative()
		.get<decltype(m_get_item_info_manager_fn)>();
	m_get_resource_manager_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 48 8B C8 45 33 C0 48 8D 55 78"), _("GetResourceManager"))
		.relative()
		.get<decltype(m_get_resource_manager_fn)>();

	/* Functions */

	// App
	m_app_update_fn = m_game_module.find_pattern(_("48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 55 41 56 41 57 48 8D 68 C8 "), _("App::Update"))
		.get<decltype(m_app_update_fn)>();
	m_set_fps_limit_fn = m_game_module.find_pattern(_("E8 ? ? ? ? E8 ? ? ? ? 83 E8 02"), _("BaseApp::SetFPSLimit"))
		.relative()
		.get<decltype(m_set_fps_limit_fn)>();
	m_log_msg_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 45 3B F7"), _("LogMsg"))
		.relative()
		.get<decltype(m_log_msg_fn)>();
	m_log_to_console_fn = m_game_module.find_pattern(_("48 89 4C 24 ? 48 89 54 24 ? 4C 89 44 24 ? 4C 89 4C 24 ? 53 57 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B D9 33 D2 48 8D 4C 24 ? 41 B8 ? ? ? ? E8 ? ? ? ? 48 8D BC 24 ? ? ? ? E8 ? ? ? ? 48 89 7C 24 ? 48 8D 54 24 ? 33 FF"), _("LogToConsole"))
		.get<decltype(m_log_to_console_fn)>();
	m_create_text_label_entity_fn = m_game_module.find_pattern(_("48 8B C4 48 89 58 ? 55 56 57 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 0F 28 F3"), _("CreateTextLabelEntity"))
		.get<decltype(m_create_text_label_entity_fn)>();

	// Component
	m_dialog_is_opened_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 48 8B 75 27"), _("GameLogicComponent::DialogIsOpened"))
		.relative()
		.get<decltype(m_dialog_is_opened_fn)>();
	m_on_text_game_message_fn = m_game_module.find_pattern(_("E8 ? ? ? ? E9 ? ? ? ? 48 8B 4F 18 E8 ? ? ? ? 48 8B D8"), _("GameLogicComponent::OnTextGameMessage"))
		.relative()
		.get<decltype(m_on_text_game_message_fn)>();
	m_process_tank_update_packet_fn = m_game_module.find_pattern(_("48 89 54 24 ? 48 89 4C 24 ? B8 ? ? ? ?"), _("GameLogicComponent::ProcessTankUpdatePacket"))
		.get<decltype(m_process_tank_update_packet_fn)>();
	m_handle_touch_at_world_coordinates_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 48 8B 8D ? ? ? ? 48 33 CC E8 ? ? ? ? 4C 8D 9C 24 ? ? ? ? 49 8B 5B 50 41 0F 28 73 ? 41 0F 28 7B ? 45 0F 28 43 ? 45 0F 28 4B ? 49 8B E3"), _("LevelTouchComponent::HandleTouchAtWorldCoordinates"))
		.relative()
		.get<decltype(m_handle_touch_at_world_coordinates_fn)>();
	m_handle_track_packet_fn = m_game_module.find_pattern(_("E8 ? ? ? ? EB 4E 48 8D 0D ? ? ? ?"), _("TrackHandleComponent::HandleTrackPacket"))
		.relative()
		.get<decltype(m_handle_track_packet_fn)>();

	// Network
	m_send_packet_fn = m_game_module.find_pattern(_("E8 ? ? ? ? E8 ? ? ? ? 48 8D 85 ? ? ? ? "), _("SendPacket"))
		.relative()
		.get<decltype(m_send_packet_fn)>();
	m_send_packet_raw_fn = m_game_module.find_pattern(_("E8 ? ? ? ? EB 50 C1 E9 18"), _("SendPacketRaw"))
		.relative()
		.get<decltype(m_send_packet_raw_fn)>();

	m_can_message_t4_fn = m_game_module.find_pattern(_("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 8B C8 E8 ? ? ? ? 39 83 ? ? ? ? 76 ? 32 C0"), _("NetAvatar::CanMessageT4"))
		.get<decltype(m_can_message_t4_fn)>();
	m_collide_fn = m_game_module.find_pattern(_("E8 ? ? ? ? 48 85 C0 74 ? ? ? ? 48 8B CB E8 ? ? ? ? B0"), _("WorldTileMap::Collide"))
		.relative()
		.get<decltype(m_collide_fn)>();

	if (m_get_base_app_fn && m_set_fps_limit_fn)
		m_set_fps_limit_fn(m_get_base_app_fn(), 0.0f);

	get_app()->GetVariant(_("move_down_primary"))->Set(0x53);
	get_app()->GetVariant(_("move_down_secondary"))->Set(0x28);
}

App* c_sdk::get_app()
{
	App* app = m_get_app_fn();

	return app;
}

BaseApp* c_sdk::get_base_app()
{
	BaseApp* base_app = m_get_base_app_fn();

	return base_app;
}

GameLogicComponent* c_sdk::get_game_logic()
{
	GameLogicComponent* game_logic = m_get_game_logic_fn();

	return game_logic;
}

Entity* c_sdk::get_entity_root()
{
	Entity* entity_root = m_get_entity_root_fn();

	return entity_root;
}

ENetClient* c_sdk::get_enet_client()
{
	ENetClient* enet_client = m_get_enet_client_fn();

	return enet_client;
}

ItemInfoManager* c_sdk::get_item_info_manager()
{
	ItemInfoManager* item_info_manager = m_get_item_info_manager_fn();

	return item_info_manager;
}

ResourceManager* c_sdk::get_resource_manager()
{
	ResourceManager* resource_manager = m_get_resource_manager_fn();

	return resource_manager;
}

void c_sdk::console_log(const char* message, ...)
{
	char buf[1024];

	va_list args;
	va_start(args, message);
	vsnprintf(buf, sizeof(buf), message, args);
	va_end(args);

	m_log_to_console_fn(message);
}

void c_sdk::send_packet(eNetMessageType type, std::string& packet, void* peer)
{
	if (!m_send_packet_fn)
		return;

	m_send_packet_fn(type, packet, get_enet_client()->m_peer);
}

void c_sdk::send_packet_raw(GameUpdatePacket* packet, int flags)
{
	if (!m_send_packet_raw_fn)
		return;

	m_send_packet_raw_fn(eNetMessageType::NET_MESSAGE_GAME_PACKET, packet, sizeof(GameUpdatePacket), 0, get_enet_client()->m_peer, flags);
}

void c_sdk::process_tank_update_packet(GameUpdatePacket* packet)
{
	if (!m_get_game_logic_fn || !m_process_tank_update_packet_fn)
		return;

	m_process_tank_update_packet_fn(get_game_logic(), packet);
}

Entity* c_sdk::create_text_label_entity(const std::string& entity_name, Vector2 pos, const std::string& text)
{
	return m_create_text_label_entity_fn(get_entity_root(), entity_name, pos.x, pos.y, text);
}