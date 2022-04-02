#pragma once 
#include "pch.h"
#include "Entity.h"
#include "Core/Log.h"

using ComponentID = size_t;

namespace Clap
{
    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity) = 0;
    };


    template<typename T>
    class ComponentArray : public IComponentArray
    {
    public:
        ComponentArray()
        {
            for(int i = 0; i < MAX_ENTITIES; i++)
            {
                m_HasComponent[i] = false;
            }
        }
        void InsertData(Entity entity, T component)
        {
            CLAP_ASSERT(!m_HasComponent[entity], "Component added to same entity more than once.");

            m_Components[entity] = component;
            m_HasComponent[entity] = true;
        }

        void RemoveData(Entity entity)
        {
            CLAP_ASSERT(m_HasComponent[entity], "Removing non-existent component.");

            m_HasComponent[entity] = false;
        }

        T& GetData(Entity entity)
        {
            CLAP_ASSERT(m_HasComponent[entity], "Retrieving non-existent component.");

            return m_Components[entity];
        }

        void EntityDestroyed(Entity entity) override
        {
            if (m_HasComponent[entity])
            {
                RemoveData(entity);
            }
        }

    private:
        std::array<T, MAX_ENTITIES> m_Components;
        std::array<bool, MAX_ENTITIES> m_HasComponent;
    };



    class ComponentManager
    {
    public:
        template<typename T>
        void RegisterComponent()
        {
            const char* typeName = typeid(T).name();

            CLAP_ASSERT(m_ComponentTypes.find(typeName) == m_ComponentTypes.end(), "Registering component type more than once.");

            m_ComponentTypes.insert({typeName, m_NextComponentType});
            m_ComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            m_NextComponentType++;
        }

        template<typename T>
        ComponentID GetComponentType()
        {
            const char* typeName = typeid(T).name();

            CLAP_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component not registered before use.");

            return m_ComponentTypes[typeName];
        }

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            GetComponentArray<T>()->InsertData(entity, component);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            GetComponentArray<T>()->RemoveData(entity);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return GetComponentArray<T>()->GetData(entity);
        }

        void EntityDestroyed(Entity entity)
        {
            for (auto const& pair : m_ComponentArrays)
            {
                auto const& component = pair.second;

                component->EntityDestroyed(entity);
            }
        }

    private:
        // Map from type string pointer to a component type
        std::unordered_map<const char*, ComponentID> m_ComponentTypes;

        // Map from type string pointer to a component array
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays;

        ComponentID m_NextComponentType;

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            const char* typeName = typeid(T).name();

            CLAP_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component not registered before use.");

            return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
        }
    };
}