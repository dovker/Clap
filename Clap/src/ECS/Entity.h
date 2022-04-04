#pragma once
#include "pch.h"

using Entity = uint32_t;
const Entity MAX_ENTITIES = 10000;
const Entity NULL_ENTITY = 0;

using EntityMask = uint64_t;


namespace Clap
{
    class EntityManager
    {
    public:
        EntityManager();

        Entity CreateEntity();

        void DestroyEntity(Entity entity);
        void SetMask(Entity entity, EntityMask mask);

        EntityMask GetMask(Entity entity);
        std::vector<Entity> GetGroup(EntityMask mask);

    private:
        std::deque<Entity> m_FreeEntities;

        std::array<EntityMask, MAX_ENTITIES> m_Masks;

        uint32_t m_Count = 1;
    };
}