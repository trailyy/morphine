#pragma once
#include <cstdint>
#include "WorldCamera.hpp"

class World;

#pragma pack(push, 1)
class WorldRenderer
{
public:
	void* m_vtable;
	World* m_world;
	pad(136);
	WorldCamera* m_camera;
};
#pragma pack(pop)