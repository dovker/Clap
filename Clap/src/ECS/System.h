#pragma once
#include "Events/Event.h"
#include "Core/Core.h"

namespace Clap
{
    class World;

    class System
    {
    public:
        System(Ref<World> world) : m_World(world) {}
        virtual ~System() {};

        virtual void OnInit() {};
        virtual void OnUpdate(double deltaTime) {};
        virtual void OnRender() {};
        virtual void OnEvent(Event& e) {};
        virtual void OnImmediateEvent(Event& e) {};
        virtual void OnDestroy() {};
        
    protected:
        Ref<World> m_World;
    };
}