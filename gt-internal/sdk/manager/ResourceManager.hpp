#pragma once
#include <map>
#include <string>
#include "../enums.hpp"

class Surface;

#pragma pack(push, 1)
class Resource
{
public:
	Surface* m_surface;
	eResourceType m_type;
};

class ResourceManager
{
public:
	void* m_vtable;
	std::map<std::string, Resource*> m_data;
};
#pragma pack(pop)