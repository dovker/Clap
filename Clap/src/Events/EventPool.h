#pragma once
#include "pch.h"
#include "Event.h"

namespace Clap {

    class EventPool
    {
    public:
        EventPool() {}

        void QueueEvent(Event e);
        Event DequeueEvent();

        bool IsEmpty();
        void AttatchCallback(std::function<void(Event&)> function) { m_CallbackFuntion = function; }

    private:
        std::deque<Event> m_Queue; //TODO: SWAP WITH RING BUFFER LATER
        std::function<void(Event&)> m_CallbackFuntion;
    };

}
