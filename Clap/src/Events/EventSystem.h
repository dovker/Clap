#pragma once
#include "pch.h"
#include "Event.h"
#include "EventPool.h"

namespace Clap {

    class EventSystem
    {
    public:
        EventSystem() {}

        static void RegisterEvent(Event e);
        static Event GetEvent();

        static bool IsEmpty();
        static void AttatchCallback(std::function<void(Event&)> function);

    private:
        static EventPool* s_Pool;
    };

}
