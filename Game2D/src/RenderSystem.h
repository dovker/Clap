#include "Clap.h"
#include "System.h"
#include "Components.h"
#include "PostProcess.h"

using namespace Clap;

namespace Game2D
{
    struct PostProcessingEffects
    {
        glm::vec3 Ambient = {1.0f, 1.0f, 1.0f};
        float Gamma = 1.0f; // 2.2 on normal
        float Exposure = 1.0f;
        float Brightness = 0.0f;
        float Contrast = 1.0f;
        float Saturation = 1.0f;
        float BlackPoint = 0.0f;
        float WhitePoint = 1.0f;
    };

    class RenderSystem : public System
    {
    public:
        PostProcessingEffects Effects;
    public:
        RenderSystem(Ref<World> world) : System(world) {}
        void OnInit() override
        {
            Graphics::Init();
            Batch::Init();

            auto framebuffer = Framebuffer::Create({1280, 720, {TextureFormat::RGBA8, TextureFormat::DEPTH24STENCIL8}, 1, false});
            auto shader = Shader::Create("../../../Game2D/res/PostProcess.glsl");
            auto uniformBuffer = UniformBuffer::Create(sizeof(PostProcessingEffects), 1);
            uniformBuffer->SetData(&Effects, sizeof(Effects));
            m_PostProcess = PostProcess::Create(framebuffer, shader, uniformBuffer);
        }

        void OnRender() override
        {
            Graphics::ToggleDepthTest(true);
            Graphics::ToggleBlending(true);
            Graphics::SetClearColor({0.0f, 0.0f, 0.0f, 0.0f});

            Graphics::Clear();

            m_PostProcess->Begin();

            glm::mat4 cameraTransform;
            glm::mat4 cameraProjection;
            
            for(Entity e : m_World->GetGroup<TransformComponent, CameraComponent>())
            {
                CameraComponent camera = m_World->GetComponent<CameraComponent>(e);
                if(camera.Primary)
                {
                    cameraTransform = m_World->GetComponent<TransformComponent>(e).GetTransform();
                    cameraProjection = camera.Projection;
                }
            }

            Batch::Begin(cameraProjection, cameraTransform);

            for(Entity e : m_World->GetGroup<TransformComponent, SpriteComponent>())
            {
                Batch::Submit(m_World->GetComponent<TransformComponent>(e).GetTransform(), m_World->GetComponent<SpriteComponent>(e).Texture);
            }
            
            Batch::End();

            m_PostProcess->End();
        }

        void OnDestroy() override
        {
            Batch::Shutdown();
        }
    private:
        Ref<PostProcess> m_PostProcess;
    };
}


