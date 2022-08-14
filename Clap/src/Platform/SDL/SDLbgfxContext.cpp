#include "pch.h"

#include "Core/Window.h"
#include "Core/Log.h"
#include "SDLbgfxContext.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_syswm.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

namespace Clap {

    SDLbgfxContext::SDLbgfxContext(Window* window)
        : m_Window(window)
    {
        #if !BX_PLATFORM_EMSCRIPTEN
            SDL_SysWMinfo wmi;
            SDL_VERSION(&wmi.version);
            if (!SDL_GetWindowWMInfo((SDL_Window*)m_Window->GetWindowPtr(), &wmi)) {
                CLAP_ERROR("SDL_SysWMinfo could not be retrieved. SDL_Error: ", SDL_GetError());
            }
            bgfx::renderFrame(); // single threaded mode
        #endif // !BX_PLATFORM_EMSCRIPTEN

            bgfx::PlatformData pd{};
        #if BX_PLATFORM_WINDOWS
            pd.nwh = wmi.info.win.window;
        #elif BX_PLATFORM_OSX
            pd.nwh = wmi.info.cocoa.window;
        #elif BX_PLATFORM_LINUX
            pd.ndt = wmi.info.x11.display;
            pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
        #elif BX_PLATFORM_EMSCRIPTEN
            pd.nwh = (void*)"#canvas";
        #endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX ?
            // BX_PLATFORM_EMSCRIPTEN

        bgfx::Init bgfxInit;
        bgfxInit.type = bgfx::RendererType::Count;
        bgfxInit.resolution.width = window->GetWidth();
        bgfxInit.resolution.height = window->GetHeight();
        bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
        bgfxInit.platformData = pd;
        bgfx::init(bgfxInit);

        CLAP_INFO("BGFX Loaded");

        // #if BX_PLATFORM_EMSCRIPTEN
        //     emscripten_set_main_loop_arg(main_loop, &context, -1, 1);
        // #else
    }

    SDLbgfxContext::~SDLbgfxContext()
    {
        bgfx::shutdown();
    }

    void SDLbgfxContext::SetSwapInterval(bool interval)
    {
        //TODO: IMPLEMENT
        interval = false;
    }

    void SDLbgfxContext::SwapBuffers()
    {
        bgfx::frame();
    }
}