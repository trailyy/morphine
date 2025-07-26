#pragma once
#include <cstdint>
#include <deque>
#include <string>
#include "Boost.hpp"

#pragma pack(push, 1)
class Console
{
public:
	void* m_vtable;
	BoostSignal m_sig_on_text_added;
	uint32_t m_max_lines;
	std::deque<std::string> m_stream;
	pad(84);
};
#pragma pack(pop)