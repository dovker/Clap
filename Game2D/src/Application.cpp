#include <glm/glm.hpp>
#include <memory>

#include "Clap.h"
#include "Components.h"
#include "System.h"

using namespace Clap;

namespace Game2D
{
    class RenderSystem : public System
    {
    public:
        RenderSystem(Ref<World> world) : System(world) {}
        void OnInit() override
        {
            Graphics::Init();
            Batch::Init();
        }

        void OnRender() override
        {
            Graphics::ToggleDepthTest(true);
            Graphics::ToggleBlending(true);
            Graphics::SetClearColor({0.4f, 0.3f, 0.6f, 1.0f});

            Graphics::Clear();

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
        }

        void OnDestroy() override
        {
            Batch::Shutdown();
        }
    };
}

namespace Game2D
{
    std::vector<std::shared_ptr<System>> s_Systems;

    void OnImmediateEvent(Event& e)
    {
        for(auto& sys : s_Systems)
        {
            sys->OnImmediateEvent(e);
        }
    }

    void Start()
    {
        EventSystem::AttatchCallback(&OnImmediateEvent);
        bool running = true;

        Window* window = Window::Create();

        Transform cameraTransform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        Transform transform(glm::vec3(0.0f, 0.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 cameraProjection = glm::ortho(-1280.0f * 0.5f, 1280.0f * 0.5f, 720.0f * 0.5f, -720.0f * 0.5f, 100.0f, -100.0f);

        Ref<Texture2D> albedo = Texture2D::Create("../../Game2D/res/textures/awesomeface.png");

        Ref<World> GameWorld = World::Create();
        GameWorld->RegisterComponent<TransformComponent>();
        GameWorld->RegisterComponent<SpriteComponent>();
        GameWorld->RegisterComponent<CameraComponent>();

        Entity camera = GameWorld->CreateEntity();
        GameWorld->AddComponent<TransformComponent>(camera, cameraTransform);
        GameWorld->AddComponent<CameraComponent>(camera, {cameraProjection, true});
        Entity sprite = GameWorld->CreateEntity();
        GameWorld->AddComponent<TransformComponent>(sprite, transform);
        GameWorld->AddComponent<SpriteComponent>(sprite, {albedo});

        s_Systems.push_back(std::make_shared<RenderSystem>(GameWorld));

        for(auto& sys : s_Systems)
        {
            sys->OnInit();
        }

        double CurrentTime = window->GetTime();
        double PreviousTime = 0.0;
        double DeltaTime;

        while(running)
        {
            PreviousTime = CurrentTime;
            CurrentTime = window->GetTime();

            DeltaTime = (double)((CurrentTime - PreviousTime)*1000 / window->GetTime() ); //SDL WAY

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
                for(auto& sys : s_Systems)
                {
                    sys->OnEvent(e);
                }
            }

            for(auto& sys : s_Systems)
            {
                sys->OnUpdate(DeltaTime);
            }

            for(auto& sys : s_Systems)
            {
                sys->OnRender();
            }
            
            window->OnUpdate();
        }
        for(auto& sys : s_Systems)
        {
            sys->OnDestroy();
        }
    }


}

int main()
{
    Game2D::Start();
    return 0;
}