#include <glm/glm.hpp>
#include <memory>

#include "Clap.h"
#include "Components.h"
#include "System.h"
#include "Systems.h"
#include "Tiles.h"


using namespace Clap;
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
        Transform transform(glm::vec3(0.0f, 0.0f, -0.01f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 cameraProjection = glm::ortho(-320.0f * 0.5f, 320.0f * 0.5f, 180.0f * 0.5f, -180.0f * 0.5f, 100.0f, -100.0f);

        Ref<Texture2D> albedo = Texture2D::Create("../../../Game2D/res/textures/sprite.png");
        Ref<Texture2D> tilemapTexture = Texture2D::Create("../../../Game2D/res/textures/Tilemap.png");

        Ref<World> GameWorld = World::Create();
        GameWorld->RegisterComponent<TransformComponent>();
        GameWorld->RegisterComponent<SpriteComponent>();
        GameWorld->RegisterComponent<CameraComponent>();
        GameWorld->RegisterComponent<CameraControllerComponent>();
        GameWorld->RegisterComponent<RPGPhysicsBodyComponent>();
        GameWorld->RegisterComponent<PlayerControllerComponent>();
        GameWorld->RegisterComponent<TileMapComponent>();


        Entity camera = GameWorld->CreateEntity();
        GameWorld->AddComponent<TransformComponent>(camera, cameraTransform);
        GameWorld->AddComponent<CameraComponent>(camera, {cameraProjection, true});
        Entity sprite = GameWorld->CreateEntity();
        GameWorld->AddComponent<TransformComponent>(sprite, transform);
        GameWorld->AddComponent<SpriteComponent>(sprite, {albedo});
        GameWorld->AddComponent<PlayerControllerComponent>(sprite, {glm::vec3(5.0f, 5.0f, 0.0f)});
        GameWorld->AddComponent<RPGPhysicsBodyComponent>(sprite, RPGPhysicsBodyComponent());
        //GameWorld->GetComponent<PhysicsBodyComponent>(sprite).Force = {-90.0f, 0.0f, 0.0f};
        Entity tilemap = GameWorld->CreateEntity();
        GameWorld->AddComponent<TransformComponent>(tilemap, cameraTransform);
        auto tiles = TileMap::Create(100, 100, 6);
        GameWorld->AddComponent<TileMapComponent>(tilemap, TileMapComponent(tiles, tilemapTexture, {8, 8}));

        GameWorld->AddComponent<CameraControllerComponent>(camera, {sprite, 0.1f, glm::vec2(0.5f)});

        s_Systems.push_back(std::make_shared<RenderSystem>(GameWorld, glm::vec2(1280, 720)));
        s_Systems.push_back(std::make_shared<CameraSystem>(GameWorld));
        s_Systems.push_back(std::make_shared<RPGPhysicsSystem>(GameWorld));
        s_Systems.push_back(std::make_shared<PlayerControllerSystem>(GameWorld));

        for(auto& sys : s_Systems)
        {
            sys->OnInit();
        }

        const double FRAME_TIME = 1 / 60.0;

        double CurrentTime = 0.0;
        double PreviousTime = window->GetTime();
        double DeltaTime;

        while(running)
        {
            PreviousTime = CurrentTime;
            CurrentTime = window->GetTime();

            DeltaTime = (double)((CurrentTime - PreviousTime) / window->GetFrequency() ); //SDL WAY

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
