#pragma once
#include <ShlObj.h>
#include <filesystem>

#include "config.hpp"
#include "../lib/json/json.h"
#include "../globals.hpp"
#include "../utils/utils.hpp"

c_config* g_config = new c_config();

std::vector<uint8_t> ReadAllBytes(char const* filename)
{
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<uint8_t> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read((char*)&result[0], pos);

	return result;
}

void c_config::add_item(void* pointer, const std::string& name, e_item_type type)
{
	m_items.emplace(name, new config_item_t(pointer, name, type));
}

void c_config::setup_item(std::string* pointer, const std::string& value, const std::string& name)
{
	add_item(pointer, name, e_item_type::TYPE_STRING);
	*pointer = value;
}

void c_config::setup_item(int* pointer, int value, const std::string& name)
{
	add_item(pointer, name, e_item_type::TYPE_INT);
	*pointer = value;
}

void c_config::setup_item(bool* pointer, bool value, const std::string& name)
{
	add_item(pointer, name, e_item_type::TYPE_BOOL);
	*pointer = value;
}

void c_config::setup_item(float* pointer, float value, const std::string& name)
{
	add_item(pointer, name, e_item_type::TYPE_FLOAT);
	*pointer = value;
}

void c_config::initialize()
{
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, NULL, path)))
	{
		m_directory = std::string(path) + _("\\Growtopia\\morphine\\");

		if (!std::filesystem::exists(m_directory))
			std::filesystem::create_directory(m_directory);
	}

	setup_item(&m_cheats.m_modfly, false, _("Cheats.Modfly"));
	setup_item(&m_cheats.m_mod_zoom, false, _("Cheats.Mod_zoom"));

	setup_item(&m_local.m_constants_editor, false, _("Local.constants_editor"));

	setup_item(&m_misc.m_debug_packets, false, _("Misc.Debug_packets"));

	refresh();

	print(_("initialized config system"));
}

void c_config::refresh()
{
	if (!std::filesystem::exists(get_config_path()) || !std::filesystem::exists(get_config_path()))
		return;

	m_configs.clear();

	for (const auto& entry : std::filesystem::directory_iterator(get_config_path()))
	{
		if (!entry.exists())
			continue;

		if (entry.path().extension() == _(".cfg"))
		{
			std::string path = entry.path().string();
			std::string replace = get_config_path();

			path.replace(path.find(replace), replace.length(), "");

			m_configs.push_back(path);
		}
	}
}

void c_config::create(std::string name)
{
	std::ofstream file(get_config_path() + name + _(".cfg"));

	if (file.good())
	{
		file << "";
		file.close();
	}

	refresh();
}

void c_config::remove(int idx)
{
	if (this->m_configs.empty() || idx >= this->m_configs.size())
		return;

	std::error_code ec;
	std::filesystem::remove(get_config_path() + this->m_configs.at(idx), ec);
	this->m_configs.erase(this->m_configs.cbegin() + idx);
}

void c_config::save(int idx)
{
	if (this->m_configs.empty() || idx >= this->m_configs.size())
		return;

	Json::Value output;

	for (auto& it : m_items)
	{
		Json::Value j;
		config_item_t* item = it.second;

		if (!item)
			continue;

		j[_("name")] = item->m_name;
		j[_("type")] = item->m_type;

		switch (item->m_type)
		{
			case e_item_type::TYPE_INT:
			{
				j[_("value")] = *(int*)item->m_ptr;
			}
			break;

			case e_item_type::TYPE_FLOAT:
			{
				j[_("value")] = *(float*)item->m_ptr;
			}
			break;

			case e_item_type::TYPE_BOOL:
			{
				j[_("value")] = *(bool*)item->m_ptr;
			}
			break;

			case e_item_type::TYPE_STRING:
			{
				j[_("value")] = *(std::string*)item->m_ptr;
			}
			break;
		}

		output.append(j);
	}

	std::string config = output.toStyledString();

	std::ofstream file(get_config_path() + this->m_configs.at(idx));
	if (file.good())
	{
		file << config;
		file.close();
	}
}

void c_config::load(int idx)
{
	if (this->m_configs.empty() || idx >= this->m_configs.size())
		return;

	std::string file = get_config_path() + m_configs.at(idx);

	std::vector<uint8_t> content = ReadAllBytes(file.c_str());
	if (content.empty()) return;

	Json::Value configuration;

	std::stringstream stream;
	stream << std::string(content.begin(), content.end());
	stream >> configuration;

	for (Json::Value::iterator it = configuration.begin(); it != configuration.end(); ++it)
	{
		Json::Value j = *it;

		std::string name = j[_("name")].asString();
		int type = j[_("type")].asInt();

		config_item_t* item = m_items[name];
		if (!item) continue;

		switch (type)
		{
			case e_item_type::TYPE_INT:
			{
				*reinterpret_cast<int*>(item->m_ptr) = j[_("value")].asInt();
			}
			break;

			case e_item_type::TYPE_FLOAT:
			{
				*reinterpret_cast<float*>(item->m_ptr) = j[_("value")].asFloat();
			}
			break;

			case e_item_type::TYPE_BOOL:
			{
				*reinterpret_cast<bool*>(item->m_ptr) = j[_("value")].asBool();
			}
			break;

			case e_item_type::TYPE_STRING:
			{
				*reinterpret_cast<std::string*>(item->m_ptr) = j[_("value")].asString();
			}
			break;
		}
	}
}