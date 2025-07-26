#pragma once
#include <list>
#include "../proton/Vector.hpp"

#pragma pack(push, 1)
class WorldObject
{
public:
    void* m_vtable;
    Vector2 m_pos;
    uint16_t m_item_id;
    uint8_t m_count;
    uint8_t m_flags;
    uint32_t m_object_id;
    uint32_t m_insert_timestamp;
    pad(20);
};
static_assert(sizeof(WorldObject) == 0x30);

class WorldObjectMap
{
public:
    void* m_vtable;
    pad(72);
    std::list<WorldObject> m_objects;
};
#pragma pack(pop)