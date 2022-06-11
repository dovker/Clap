#include "Clap.h"

using namespace Clap;

int main()
{

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
                default: break;
            }
        }
        window->OnUpdate();
    }

    return 0;
}