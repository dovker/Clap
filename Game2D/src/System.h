#pragma once
#include "Clap.h"

using namespace Clap;

namespace Game2D
{
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