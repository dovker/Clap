#include "Clap.h"

#include "bgfx/bgfx.h"


using namespace Clap;

const std::string shaderPath = "../../res/shaders/compiled/";

bgfx::ShaderHandle loadShader(std::string path)
{
    std::string platform = "???";

    switch(bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:  platform = shaderPath + "dx9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: platform = shaderPath + "dx11/";  break;
        case bgfx::RendererType::Agc:
        case bgfx::RendererType::Gnm:        platform = shaderPath + "pssl/";  break;
        case bgfx::RendererType::Metal:      platform = shaderPath + "metal/"; break;
        case bgfx::RendererType::Nvn:        platform = shaderPath + "nvn/";   break;
        case bgfx::RendererType::OpenGL:     platform = shaderPath + "glsl/";  break;
        case bgfx::RendererType::OpenGLES:   platform = shaderPath + "essl/";  break;
        case bgfx::RendererType::Vulkan:     platform = shaderPath + "spirv/"; break;
        case bgfx::RendererType::WebGPU:     platform = shaderPath + "spirv/"; break;

        case bgfx::RendererType::Count:
            CLAP_ASSERT(false, "RENDERER TYPE NOT FOUND!");
            break;
    }
    auto temp = platform + path;
    const char* filePath = temp.c_str();

    FILE *file = fopen(filePath, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

int main()
{
    bool running = true;
    Window* window = Window::Create();

    bgfx::VertexLayout layout;
    layout.begin()
          .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
          .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, true)
          .end();

    //Graphics::Init();

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 1280, 720);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    uint32_t indices[] = {
        0, 1, 2
    };

    auto vb = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), layout);
    auto ib = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));

    bgfx::ShaderHandle vsh = loadShader("vs_cubes.bin");
    bgfx::ShaderHandle fsh = loadShader("fs_cubes.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    uint64_t state = 0
				| BGFX_STATE_WRITE_RGB
                | BGFX_STATE_WRITE_A
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_CULL_CW
				| BGFX_STATE_MSAA
				| BGFX_STATE_PT_TRISTRIP
				;

    glm::mat4 view(1.0f);
    glm::mat4 proj(1.0f);

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

            bgfx::setVertexBuffer(0, vb);
			bgfx::setIndexBuffer(ib);

            bgfx::setViewTransform(0, &view, &proj);

            bgfx::setState(state);

            bgfx::submit(0, program);
            //bgfx::frame();
        }
        window->OnUpdate();
    }

    bgfx::destroy(vb);
    bgfx::destroy(ib);
	bgfx::destroy(program);

    return 0;
}