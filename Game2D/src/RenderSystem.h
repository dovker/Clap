#include "Clap.h"
#include "System.h"
#include "Components.h"
#include "PostProcess.h"
#include "Tiles.h"


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

    
    glm::vec4 GetTileUVCoords(const TileMapComponent& tileMap, uint16_t ID)
    {
        uint32_t tW = tileMap.Texture->GetWidth();
        uint32_t tH = tileMap.Texture->GetHeight();
        uint32_t tileSizeX = tileMap.TileSize.x;
        uint32_t tileSizeY = tileMap.TileSize.y;

        float sY = (ID * tileSizeY / tW);
        float sX = ((ID * tileSizeX) % tW) / tileSizeX;
        float sX1 = sX+1;
        float sY1 = sY+1;

        glm::vec4 texCoords = {sX * tileSizeX / tW, 1.0f - sY * tileSizeY / tH, sX1 * tileSizeX/ tW, 1.0f - sY1 * tileSizeY/tH};

        return texCoords;
    }

    void RenderTileMap(const Transform& transform, TileMapComponent& component)
    {
        //TODO: CHUNK LOADING RENDERING
        for(uint32_t i = 0; i < component.Map->GetWidth(); i++)
        {
            for(uint32_t j = 0; j < component.Map->GetHeigth(); j++)
            {
                Tile t = component.Map->At(i, j);
                if(t.ID > 0)
                {
                    Transform trans = Transform(transform);
                    glm::vec2 tileSize = component.TileSize * glm::vec2(transform.Scale.x, transform.Scale.y);
                    Batch::Submit({trans.Position.x + tileSize.x * i, trans.Position.y + tileSize.y * j, trans.Position.z}, tileSize, component.Texture, 
                                GetTileUVCoords(component, t.ID), glm::vec4(1.0f));
                }
            }
        }
    }

    class RenderSystem : public System
    {
    public:
        PostProcessingEffects Effects;
    public:
        RenderSystem(Ref<World> world, glm::vec2 windowSize) : System(world), m_WindowSize(windowSize) {}
        void OnInit() override
        {
            Graphics::Init();
            Batch::Init();

            auto framebuffer = Framebuffer::Create({640, 360, {{TextureFormat::RGBA8, FilterType::NEAREST, FilterType::NEAREST}, TextureFormat::DEPTH24STENCIL8}, 1, false});
            auto shader = Shader::Create("../../../Game2D/res/PostProcess.glsl");
            auto uniformBuffer = UniformBuffer::Create(sizeof(PostProcessingEffects), 1);
            uniformBuffer->SetData(&Effects, sizeof(Effects));
            m_PostProcess = PostProcess::Create(m_WindowSize, framebuffer, shader, uniformBuffer);
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
            //TODO: SORT BY DEPTH
            //TODO: MAKE CAMERAOUTSIDE OF THE FRAMEBUFFER FOR SMOOTHER MOVEMENT
            for(Entity e : m_World->GetGroup<TransformComponent, SpriteComponent>())
            {
                Batch::Submit(m_World->GetComponent<TransformComponent>(e).GetTransform(), m_World->GetComponent<SpriteComponent>(e).Texture);
            }
            for(Entity e : m_World->GetGroup<TransformComponent, TileMapComponent>())
            {
                RenderTileMap(m_World->GetComponent<TransformComponent>(e), m_World->GetComponent<TileMapComponent>(e));
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
        glm::vec2 m_WindowSize;
    };
}


