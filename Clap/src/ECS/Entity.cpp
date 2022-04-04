#include "Entity.h"
#include "Core/Log.h"
#include "Core/Core.h"

namespace Clap
{
    EntityManager::EntityManager()
    {
        for (Entity entity = 1; entity < MAX_ENTITIES; ++entity)
        {
            m_FreeEntities.push_back(entity);
            m_Masks[entity] = 0;
        }
    }

    Entity EntityManager::CreateEntity()
    {
        CLAP_ASSERT(m_Count < MAX_ENTITIES, "Too many entities created.");

        Entity id = m_FreeEntities.front();
        m_FreeEntities.pop_front();

        if(id >= m_Count)
            m_Count++;

        return id;
    }

    void EntityManager::DestroyEntity(Entity entity)
    {
        CLAP_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

        m_Masks[entity] = 0;

        m_FreeEntities.push_front(entity);
    }

    void EntityManager::SetMask(Entity entity, EntityMask mask)
    {
        CLAP_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

        m_Masks[entity] = mask;
    }

    EntityMask EntityManager::GetMask(Entity entity)
    {
        CLAP_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

        return m_Masks[entity];
    }

    std::vector<Entity> EntityManager::GetGroup(EntityMask mask)
    {
        CLAP_ASSERT(mask != 0, "ERROR, Mask cannot be 0");
        std::vector<Entity> group;
        for(Entity i = 1; i < m_Count; i++)
        {
            if((m_Masks[i] & mask) == mask)
            {
                group.push_back(i);
            }
        }
        return group;
    }
}