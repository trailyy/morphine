#pragma once
#include "menu.hpp"
#include <Windows.h>
#include "../globals.hpp"
#include "../hooks/render/renderer/renderer.hpp"

#include "../config/config.hpp"

c_menu* g_menu = new c_menu();

bool c_menu::initialize()
{
	if (m_initialized)
		return false;

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(g_globals->m_dll.m_window);
	ImGui_ImplOpenGL3_Init();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_NoMouseCursorChange;
	io.IniFilename = nullptr;

	m_drawlist = ImGui::GetBackgroundDrawList();

	RECT window_size;
	GetWindowRect(g_globals->m_dll.m_window, &window_size);

	m_screen_size.x = window_size.right - window_size.left;
	m_screen_size.y = window_size.bottom - window_size.top;

	io.FontDefault = io.Fonts->AddFontFromFileTTF(_("c:/windows/fonts/verdana.ttf"), 14.0f);

	m_initialized = true;

	// ghetto watermark
	Entity* morphine = g_sdk->create_text_label_entity(_("morphine_watermark"), Vector2(30.f, m_screen_size.y - 100.0f), _("morphine"));

	morphine->GetComponentByName(_("TextRender"))->GetVariant(_("font"))->Set(1);
	morphine->GetComponentByName(_("TextRender"))->GetVariant(_("style"))->Set(1);
	morphine->GetVariant(_("alignment"))->Set(4);

	return true;
}

Entity* selected_entity = 0;
bool found_entity = false;

void draw_entity(Entity* entity)
{
	ImGui::BeginGroup();

	bool this_selected = selected_entity == entity;

	if (entity->m_children.empty())
	{
		ImGui::Bullet();
		ImGui::Selectable(entity->m_name.c_str());

		if (ImGui::IsItemClicked())
			selected_entity = entity;
	}
	else
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

		if (this_selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool open = ImGui::TreeNodeEx(entity->m_name.c_str(), flags);

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			selected_entity = entity;

		if (open)
		{
			for (Entity* child : entity->m_children)
				draw_entity(child);

			ImGui::TreePop();
		}
	}

	if (selected_entity == entity)
		found_entity = true;

	ImGui::EndGroup();
}

void draw_variants(const VariantDB& db)
{
	for (const auto& entry : db.m_data)
	{
		Variant* variant = entry.second;

		if (ImGui::TreeNode(entry.first.c_str()))
		{
			switch (variant->GetType())
			{
				case eVariantType::FLOAT:
					ImGui::InputFloat(_("value##float"), &variant->GetFloat());
					break;

				case eVariantType::UINT32:
				case eVariantType::INT32:
					ImGui::InputInt(_("value##int"), &variant->GetInt());
					break;

				case eVariantType::STRING:
				{
					char* text = const_cast<char*>(variant->GetString().c_str());
					ImGui::InputText(_("value##string"), text, 256);

					if (ImGui::Button(_("update##string")))
						variant->Set(text);

					break;
				}

				case eVariantType::VECTOR2:
					ImGui::InputFloat(_("x value##vec2"), &variant->GetVec2().x);
					ImGui::InputFloat(_("y value##vec2"), &variant->GetVec2().y);
					break;

				case eVariantType::VECTOR3:
					ImGui::InputFloat(_("x value##vec3"), &variant->GetVec3().x);
					ImGui::InputFloat(_("y value##vec3"), &variant->GetVec3().y);
					ImGui::InputFloat(_("z value##vec3"), &variant->GetVec3().z);
					break;

				case eVariantType::RECT:
					ImGui::InputFloat(_("x value##vec4"), &variant->GetVec4().x);
					ImGui::InputFloat(_("y value##vec4"), &variant->GetVec4().y);
					ImGui::InputFloat(_("z value##vec4"), &variant->GetVec4().z);
					ImGui::InputFloat(_("w value##vec4"), &variant->GetVec4().w);
					break;

				default:
					ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, _("<unknown>"));
					break;
			}

			ImGui::TreePop();
		}
	}
}

void draw_components(EntityComponent* entity_component)
{
	const std::string& name = g_utils->format(_("%s (0x%llx)"), entity_component->m_name->c_str(), entity_component);

	if (entity_component->m_shared_db.m_data.empty())
		ImGui::BulletText(name.c_str());
	else
	{
		if (ImGui::TreeNode(name.c_str()))
		{
			draw_variants(entity_component->m_shared_db);
			ImGui::TreePop();
		}
	}
}

void c_menu::render()
{
	auto& io = ImGui::GetIO();
	io.MouseDrawCursor = false;

	if (!m_open)
		return;

	ImGui::SetNextWindowSize({ 500, 350 }, ImGuiCond_::ImGuiCond_Once);
	ImGui::SetNextWindowPos({ 150, 150 }, ImGuiCond_::ImGuiCond_Once);

	ImGui::Begin(_("morphine"), NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	{
		if (ImGui::BeginTabBar(_("tabs"), NULL))
		{
			if (ImGui::BeginTabItem(_("cheats")))
			{
				ImGui::Checkbox(_("modfly"), &g_config->m_cheats.m_modfly);
				ImGui::Checkbox(_("mod zoom"), &g_config->m_cheats.m_mod_zoom);

				ImGui::EndTabItem();
			}	

			if (ImGui::BeginTabItem(_("visuals")))
			{
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(_("misc")))
			{
				ImGui::Checkbox(_("debug packets"), &g_config->m_misc.m_debug_packets);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(_("player")))
			{
				NetAvatar* net_avatar = g_sdk->get_game_logic()->m_local_player;
				ImGui::Text(_("netavatar: 0x%llx"), g_sdk->get_game_logic()->m_local_player);

				if (net_avatar)
				{
					ImGui::Text(_("m_pos: %s"), net_avatar->m_pos.dump().c_str());
					ImGui::Text(_("m_size: %s"), net_avatar->m_size.dump().c_str());
					ImGui::Text(_("m_name: %s"), net_avatar->m_name.c_str());
					ImGui::Text(_("m_net_id: %d"), net_avatar->m_net_id);
					ImGui::Text(_("m_facing_left: %d"), net_avatar->m_facing_left);
					ImGui::Text(_("m_jumping: %d"), net_avatar->m_jumping);
					ImGui::Text(_("m_on_solid: %d"), net_avatar->m_on_solid);
					ImGui::Text(_("m_sliding: %d"), net_avatar->m_sliding);
					ImGui::Text(_("m_velocity: x: %.f y: %.f"), net_avatar->m_velocity.x.Get(), net_avatar->m_velocity.y.Get());
					ImGui::Text(_("m_emotion: %d"), net_avatar->m_emotion_type);
					ImGui::Text(_("m_emotion_time: %.f"), net_avatar->m_emotion_time);
					ImGui::Text(_("m_user_id: %d"), net_avatar->m_user_id);
					ImGui::Text(_("m_country: %s"), net_avatar->m_country.c_str());
					ImGui::Text(_("m_flags: %d"), net_avatar->m_flags);
					ImGui::Text(_("m_is_invis: %d"), net_avatar->m_is_invis);
					ImGui::Text(_("m_is_mod: %d"), net_avatar->m_is_mod);
					ImGui::Text(_("m_is_smod: %d"), net_avatar->m_is_smod);
					ImGui::Text(_("m_bubble_state: %d"), net_avatar->m_bubble_state);
					ImGui::Text(_("m_state: %d"), net_avatar->m_state);
					ImGui::Text(_("m_down_key_state: %d"), net_avatar->m_down_key_state);
					ImGui::Text(_("m_punch_effect: %d"), net_avatar->m_punch_effect);
					ImGui::Text(_("m_gravity: %.f"), net_avatar->m_gravity.Get());
					ImGui::Text(_("m_acceleration: %.f"), net_avatar->m_acceleration.Get());
					ImGui::Text(_("m_speed: %.f"), net_avatar->m_speed.Get());
				}
				else
					ImGui::Text(_("connect to server"));

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(_("world")))
			{
				World* world = g_sdk->get_game_logic()->m_world;
				ImGui::Text(_("world: 0x%llx"), world);

				if (world)
				{
					ImGui::Text(_("world name: %s"), world->m_name.c_str());
					ImGui::Text(_("world version: %d"), world->m_version);
					ImGui::Text(_("world size: %s"), world->m_tile_map.m_size.dump());
					ImGui::Text(_("tile map size: %d"), world->m_tile_map.m_tile_map.size());
					ImGui::Text(_("world objects size: %d"), world->m_object_map.m_objects.size());
					ImGui::Text(_("weather: %d/%d"), world->m_weather, world->m_weather_banners);
				}
				else
					ImGui::Text(_("connect to server"));

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(_("items")))
			{
				static int itemdb_current_item;

				std::vector<ItemInfo>& items = g_sdk->get_item_info_manager()->m_items;

				static ImGuiTextFilter item_filter;
				item_filter.Draw(_("search item"));

				if (!items.empty())
				{
					if (ImGui::BeginListBox(_("##itemdb"), { 0, -FLT_MIN }))
					{
						for (int i = 0; i < items.size(); i++)
						{
							const ItemInfo& item = items[i];
							const std::string& name = item.m_name;

							if (item_filter.PassFilter(name.c_str()))
							{
								const bool is_selected = (itemdb_current_item == i);
								if (ImGui::Selectable((name + _("##") + std::to_string(item.m_id)).c_str(), is_selected))
									itemdb_current_item = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
						}

						ImGui::EndListBox();
					}
				}
				else
					ImGui::Text(_("connect to server"));

				if (itemdb_current_item != 0)
				{
					const ItemInfo& item = items[itemdb_current_item];

					ImGui::SeparatorText(_("item info"));
					ImGui::Text(_("id: %d"), item.m_id);
					ImGui::Text(_("type: %d"), item.m_type);
					ImGui::Text(_("material: %d"), item.m_material);
					ImGui::Text(_("body part: %d"), item.m_body_part);
					ImGui::Text(_("flags: %d"), item.m_flags);
					ImGui::Text(_("texture: %s"), item.m_texture_path.c_str());
					ImGui::Text(_("break hits: %d"), item.m_hits_to_destroy);
					ImGui::Text(_("growtime: %d"), item.m_grow_time);
					ImGui::Text(_("rarity: %d"), item.m_rarity);
					ImGui::Text(_("max amount: %d"), static_cast<int>(item.m_max_can_hold));
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(_("entity browser")))
			{
				draw_entity(g_sdk->get_entity_root());

				if (!found_entity)
					selected_entity = nullptr;

				ImGui::SeparatorText(_("properties"));

				if (selected_entity)
				{
					ImGui::Text(_("entity %s"), selected_entity->m_name.c_str());
					
					if (selected_entity->m_parent)
						ImGui::Text(_("parent: %s"), selected_entity->m_parent->m_name.c_str());

					if (ImGui::TreeNode(_("variants")))
					{
						draw_variants(selected_entity->m_shared_db);
						ImGui::TreePop();
					}

					if (!selected_entity->m_components.empty())
					{
						if (ImGui::TreeNode(_("components")))
						{
							for (EntityComponent* component : selected_entity->m_components)
								draw_components(component);

							ImGui::TreePop();
						}
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(_("info")))
			{
				ImGui::Text(_("morphine - made by trail & voidzero -"));
				ImGui::SameLine();
				ImGui::TextLinkOpenURL(_("https://github.com/trailyy/morphine/"));
				ImGui::Text(_("this project was, and will always be free, if you paid for this, you got scammed"));
				ImGui::Text(_("licensed under CC BY-NC-SA 4.0"));

				ImGui::Separator();

				ImGui::Text(_("base address: 0x%llx"), g_globals->m_base_address);
				ImGui::Text(_("app: 0x%llx"), g_sdk->get_app());
				ImGui::Text(_("baseapp: 0x%llx"), g_sdk->get_base_app());
				ImGui::Text(_("game logic comp: 0x%llx"), g_sdk->get_game_logic());
				ImGui::Text(_("entity root: 0x%llx"), g_sdk->get_entity_root());
				ImGui::Text(_("enet client: 0x%llx"), g_sdk->get_enet_client());
				ImGui::Text(_("iteminfomanager: 0x%llx (%d)"), g_sdk->get_item_info_manager(), (int)g_sdk->get_item_info_manager()->m_items.size());


				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(_("config")))
			{
				static int current_config_item;
				static ImGuiTextFilter filter;
				filter.Draw();

				std::string config_name = filter.InputBuf;

				if (ImGui::BeginListBox(_("##configs"), { 0, -FLT_MIN }))
				{
					for (int n = 0; n < g_config->get_configs().size(); n++)
					{
						if (filter.PassFilter(g_config->get_configs().at(n).c_str()))
						{
							const bool is_selected = (current_config_item == n);

							if (ImGui::Selectable(g_config->get_configs().at(n).c_str(), is_selected))
								current_config_item = n;

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndListBox();

					if (current_config_item != -1)
					{
						if (ImGui::Button(_("Load selected")))
							g_config->load(current_config_item);

						if (ImGui::Button(_("Save selected")))
							g_config->save(current_config_item);

						if (ImGui::Button(_("Delete selected")))
							g_config->remove(current_config_item);
					}
					
					if (!config_name.empty())
					{
						if (ImGui::Button(_("Create")))
							g_config->create(config_name);
					}

					if (ImGui::Button(_("Refresh configs")))
						g_config->refresh();

					if (ImGui::Button(_("Open config directory")))
						ShellExecuteA(NULL, _("open"), g_config->get_config_path().c_str(), NULL, NULL, SW_SHOWNORMAL);
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}