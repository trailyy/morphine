#pragma once

#pragma pack(push, 1)
class AvatarData
{
public:
	void* m_vtable;
	int m_skin_color; // 8
	int m_death_animation; // 12
	int m_respawn_animation; // 16
	short m_clothing[9];
	pad(18);
};
#pragma pack(pop)