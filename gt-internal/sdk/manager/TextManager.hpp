#pragma once
#include <cstdint>

#pragma pack(push, 1)
class TextManager
{
public:
	void* m_vtable;
	pad(40);
};
#pragma pack(pop)