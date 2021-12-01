#include <glm/glm.hpp>

#include "Clap.h"

using namespace Clap;

bool running = true;

void OnEvent(Event& e)
{
    switch(e.Type)
    {
        case EventType::WindowClose:
            running = false;
        break;
        default: break;
    }
}

int main()
{
    EventSystem::AttatchCallback(&OnEvent);

    Window* window = Window::Create();
    Graphics::Init();

    while(running)
    {
        while(!EventSystem::IsEmpty())
        {
            Event e = EventSystem::GetEvent();
            switch(e.Type)
            {
                case EventType::WindowClose:
                    running = false;
                break;
                case EventType::KeyPressed:
                    CLAP_TRACE(e.Data.KeyEvent.KeyCode);
                break;
                default: break;
            }
        }
        Graphics::SetClearColor({0.3f, 0.53f, 0.5f, 1.0f});
        Graphics::Clear();

        window->OnUpdate();
    }

    return 0;
}