#pragma once
#include <cstdint>

#pragma pack(push, 1)
class TileSheetManager
{
public:
	void* m_vtable;
	pad(24);
};
#pragma pack(pop)