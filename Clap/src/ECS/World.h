#pragma once
#include "pch.h"
#include "Entity.h"
#include "Component.h"
#include "Core/Core.h"
#include "Core/Log.h"


namespace Clap
{
    class World
    {
    public:
        World()
        {
            m_ComponentManager = CreateRef<ComponentManager>();
            m_EntityManager = CreateRef<EntityManager>();
        }

        Entity CreateEntity()
        {
            return m_EntityManager->CreateEntity();
        }

        void DestroyEntity(Entity entity)
        {
            m_EntityManager->DestroyEntity(entity);
            m_ComponentManager->EntityDestroyed(entity);
        }


        template<typename T>
        void RegisterComponent()
        {
            m_ComponentManager->RegisterComponent<T>();
        }

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            m_ComponentManager->AddComponent<T>(entity, component);

            auto mask = m_EntityManager->GetMask(entity);
            mask = mask | BIT(m_ComponentManager->GetComponentType<T>());
            m_EntityManager->SetMask(entity, mask);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            m_ComponentManager->RemoveComponent<T>(entity);

            auto mask = m_EntityManager->GetMask(entity);
            mask = mask & -BIT(m_ComponentManager->GetComponentType<T>());
            m_EntityManager->SetMask(entity, mask);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return m_ComponentManager->GetComponent<T>(entity);
        }

        template<typename T>
        ComponentID GetType()
        {
            return m_ComponentManager->GetComponentType<T>();
        }

        template<typename T>
        EntityMask Mask()
        {
            return BIT(m_ComponentManager->GetComponentType<T>());
        }

        template<typename T1, typename T2, typename... Types>
        EntityMask Mask()
        {
            EntityMask mask = 0;
            mask = BIT(m_ComponentManager->GetComponentType<T1>()) | Mask<T2, Types...>();
            return mask;
        }

        template<typename... Types>
        std::vector<Entity> GetGroup()
        {
            return m_EntityManager->GetGroup(Mask<Types...>());
        }

        std::vector<Entity> GetGroup(EntityMask mask)
        {
            return m_EntityManager->GetGroup(mask);
        }

        static Ref<World> Create()
        {
            return CreateRef<World>();
        }

    private:
        Ref<ComponentManager> m_ComponentManager;
        Ref<EntityManager> m_EntityManager;
    };
}