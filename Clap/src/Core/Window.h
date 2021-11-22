#pragma once

namespace Clap
{
    class Window
    {
    public:
        
    private:

    };
}

/*
typedef struct McWindow
{
    const char *title;
    u32 width, height;
    b8 fullscreen, running;

    void *window_handle;
    void *gl_context;

} McWindow;

void window_create(McWindow *window, const char* title, u32 width, u32 height);
void window_update(McWindow *window);
void window_change_title(McWindow *window, const char* title);
u32  window_get_time();
void window_toggle_vsync(b8 toggle);
void window_toggle_fulscreen(McWindow *window, b8 toggle);
void window_destroy(McWindow *window);

void _poll_events(McWindow *window);
void _create_graphics_context(McWindow *window);
void _destroy_graphics_context(McWindow *window);
void _set_graphics_swap_interval(b8 toggle);
void _swap_graphics_buffers(McWindow *window);
void _force_modern_api();
