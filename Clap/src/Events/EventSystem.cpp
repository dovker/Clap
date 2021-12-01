#include "pch.h"
#include "EventSystem.h"

namespace Clap
{
    EventPool* EventSystem::s_Pool = new EventPool();
    void EventSystem::RegisterEvent(Event e) { s_Pool->QueueEvent(e); }
    Event EventSystem::GetEvent() { return s_Pool->DequeueEvent(); }

    bool EventSystem::IsEmpty() { return s_Pool->IsEmpty(); }
    void EventSystem::AttatchCallback(std::function<void(Event&)> function) { s_Pool->AttatchCallback(function); }
}