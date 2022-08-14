#include "Clap.h"

#include "bgfx/bgfx.h"


using namespace Clap;

int main()
{
    bool running = true;
    Window* window = Window::Create();

    //Graphics::Init();

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 1280, 720);

    while(running)
    {
        while(!EventSystem::IsEmpty())
        {
            bgfx::touch(0);
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