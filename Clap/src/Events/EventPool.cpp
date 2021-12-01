#include "pch.h"
#include "EventPool.h"

namespace Clap
{
    void EventPool::QueueEvent(Event e) 
    { 
        if(e.Immediate)
            m_CallbackFuntion(e);
        else
            m_Queue.push_back(e); 
    }

    Event EventPool::DequeueEvent()
    {
        Event e = m_Queue.front();
        m_Queue.pop_front();
        return e;
    }

    bool EventPool::IsEmpty()
    {
        return m_Queue.size() <= 0;
    }
}