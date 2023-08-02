#include "pch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "backends/imgui_impl_opengl3.cpp"
#if CLAP_USE_SDL3
#include "backends/imgui_impl_sdl3.h"
#endif