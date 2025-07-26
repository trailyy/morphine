#pragma once
#include "../globals.hpp"

#include "module.hpp"

std::vector<int32_t> pattern_to_bytes(std::string pattern)
{
	std::vector<int32_t> bytes;

	char* start = const_cast<char*>(pattern.data());
	char* end = start + pattern.size();

	for (char* c = start; c < end; ++c)
	{
		if (*c == '?')
		{
			++c;

			if (*c == '?') ++c;

			bytes.push_back(-1);
		}
		else
		{
			bytes.push_back(static_cast<int32_t>(std::strtoul(c, &c, 16)));
		}
	}

	return bytes;
};

c_address c_module::get_base_address()
{
	c_address ret = 0;

	if (!is_loaded())
		return ret;

	ret = GetModuleHandleA(m_name);

	return ret;
}

c_address c_module::get_proc_address(const char* proc_name)
{
	c_address ret = 0;

	if (!is_loaded())
		return ret;

	ret = GetProcAddress(m_module, proc_name);

	return ret;
}

c_address c_module::find_pattern(std::string pattern, std::string name)
{
	c_address ret = 0;

	if (!is_loaded())
		return ret;

	std::vector<int32_t> bytes = pattern_to_bytes(pattern);
	for (std::size_t i = m_start; i < m_end; ++i)
	{
		for (std::size_t j = 0; j < bytes.size(); ++j)
		{
			if (*reinterpret_cast<uint8_t*>(i + j) != bytes[j] && bytes[j] != -1)
				break;

			if (j == bytes.size() - 1)
			{
				ret = i;
				print(_("found %s @ 0x%llx"), name.c_str(), ret.get());

				break;
			}
		}

		if (ret.is_valid())
			break;
	}

	if (!ret.is_valid())
		print(_("failed to find %s"), name.c_str());

	return ret;
}