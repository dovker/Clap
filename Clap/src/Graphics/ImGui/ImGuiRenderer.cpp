#include "pch.h"

#include "ImGuiRenderer.h"
#include "imgui.h"

#include "Graphics/API/API.h"

#include "backends/imgui_impl_opengl3.h"

#if CLAP_USE_SDL3
#include "backends/imgui_impl_sdl3.h"
#include "SDL3/SDL.h"
#endif

namespace Clap
{
    bool ImGuiRenderer::m_BlockEvents = true;
    bool ImGuiRenderer::m_Initialized = false;
    void ImGuiRenderer::SetEventBlocking(bool blocking) { m_BlockEvents = blocking; }
    bool ImGuiRenderer::IsInitialized() { return m_Initialized; }

    void ImGuiRenderer::Init(Window* window)
    {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SDL_Window* win = static_cast<SDL_Window*>(window->GetWindowPtr());

		ImGui_ImplSDL3_InitForOpenGL(win, window->GetContextPtr());
		ImGui_ImplOpenGL3_Init("#version 450");
        m_Initialized = true;
    }

    bool ImGuiRenderer::HandleSDLEvent(SDL_Event* e)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplSDL3_ProcessEvent((e));
        
		return (io.WantCaptureMouse || io.WantCaptureKeyboard) && m_BlockEvents;
    }

    void ImGuiRenderer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
    }

    void ImGuiRenderer::End()
    {
        ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
        // Graphics::SetViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        // Graphics::SetClearColor({0.45f, 0.55f, 0.60f, 1.0f});
        // Graphics::Clear();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }

    void ImGuiRenderer::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        m_Initialized = false;
    }

}
