#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "../utils/utils.hpp"

enum e_item_type
{
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_BOOL,
	TYPE_STRING
	// i WOULD add all the types but i aint leaking all the sauce we use in xtc
};

struct config_item_t
{
	void* m_ptr;
	std::string m_name;
	int m_type;

	config_item_t(void* pointer, std::string name, e_item_type type)
	{
		this->m_ptr = pointer;
		this->m_name = name;
		this->m_type = type;
	};
};

class c_config
{
public:
	struct cheats_t
	{
		bool m_modfly;
		bool m_mod_zoom;
	} m_cheats;

	struct local_t
	{
		bool m_constants_editor;
	} m_local;

	struct misc_t
	{
		bool m_debug_packets;
	} m_misc;

	void initialize();

	void refresh();
	void create(std::string);
	void remove(int);
	void save(int);
	void load(int);

	std::string get_config_path() const { return m_directory; }
	std::map<std::string, config_item_t*> get_items() const { return m_items; };
	std::vector<std::string> get_configs() const { return m_configs; };

	void add_item(void* pointer, const std::string& name, e_item_type);
	void setup_item(std::string* pointer, const std::string& value, const std::string& name);
	void setup_item(int* pointer, int value, const std::string& name);
	void setup_item(bool* pointer, bool value, const std::string& name);
	void setup_item(float* pointer, float value, const std::string& name);

private:
	std::string m_directory; // %LocalAppData%\\Growtopia\\morphine
	std::map<std::string, config_item_t*> m_items;
	std::vector<std::string> m_configs;
};

extern c_config* g_config;