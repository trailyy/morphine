#pragma once
#include "../component/EntityComponent.hpp"
#include "../variant/VariantDB.hpp"

#include <cstdint>
#include <string>
#include <list>

#pragma pack(push, 1)
class Entity
{
public:
    void* m_vtable;
    BoostTrackableSignal m_trackable_signal;
    BoostSignal m_sig_on_removed;
    std::string m_name;
    std::list<Entity*> m_children;
    uint8_t pad[80];
    std::list<EntityComponent*> m_components;
    uint8_t pad2[80];
    VariantDB m_shared_db;
    Entity* m_parent;
    bool m_delete;

    Entity* GetEntityByName(const std::string& name)
    {
        auto it = std::find_if(m_children.begin(), m_children.end(), [&](Entity* entity) { return entity->m_name == name; });

        if (it != m_children.end())
            return *it;

        return nullptr;
    }

    EntityComponent* GetComponentByName(const std::string& name)
    {
        auto it = std::find_if(m_components.begin(), m_components.end(), [&](EntityComponent* component) { return *component->m_name == name; });

        if (it != m_components.end())
            return *it;

        return nullptr;
    }

    Variant* GetVariant(const std::string& name)
    {
        return m_shared_db.GetVariant(name);
    }

    FunctionObject* GetFunction(const std::string& name)
    {
        return m_shared_db.GetFunction(name);
    }
};
#pragma pack(pop)

