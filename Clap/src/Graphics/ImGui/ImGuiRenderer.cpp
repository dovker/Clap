#include "pch.h"

#include "ImGuiRenderer.h"
#include "ImGui.h"

#if CLAP_USE_OPENGL
#include "backends/imgui_impl_opengl3.h"
#endif

#if CLAP_USE_SDL3
#include "backends/imgui_impl_sdl3.h"
#include "SDL3/SDL.h"
#endif

namespace Clap
{
    bool ImGuiRenderer::m_BlockEvents = true;
    void ImGuiRenderer::SetEventBlocking(bool blocking) { m_BlockEvents = blocking; }

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
		ImGui_ImplSDL3_NewFrame(SDL_GL_GetCurrentWindow());
		ImGui::NewFrame();
    }

    void ImGuiRenderer::End()
    {
        ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
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
    }

}