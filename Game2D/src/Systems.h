#include "Clap.h"
#include "System.h"
#include "Components.h"

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

    class CameraSystem : public System
    {
    public:
        CameraSystem(Ref<World> world) : System(world) {}

        void OnUpdate(double deltaTime) override
        {
            for(Entity e : m_World->GetGroup<TransformComponent, CameraControllerComponent>())
            {
                TransformComponent& transform = m_World->GetComponent<TransformComponent>(e);
                CameraControllerComponent cameraController = m_World->GetComponent<CameraControllerComponent>(e);
                if(cameraController.Target != NULL_ENTITY)
                {
                    Transform target = m_World->GetComponent<TransformComponent>(cameraController.Target);
                    transform.Position = glm::mix(transform.Position, target.Position + glm::vec3(cameraController.Offset, 0.0f), cameraController.FollowVelocity);
                }
            }
        }
    };

    class PlayerControllerSystem : public System
    {
    public:
        PlayerControllerSystem(Ref<World> world) : System(world) {}

        void OnUpdate(double deltaTime) override
        {
            for(Entity e : m_World->GetGroup<TransformComponent, PlayerControllerComponent, PhysicsBodyComponent>())
            {
                TransformComponent& transform = m_World->GetComponent<TransformComponent>(e);
                PhysicsBodyComponent& body = m_World->GetComponent<PhysicsBodyComponent>(e);
                
            }
        }

    private:
        glm::vec2 InputX;
        glm::vec2 InputY;
    };

    class PhysicsSystem : public System
    {
    public:
        static float AccelerationMultiplier; 
        PhysicsSystem(Ref<World> world) : System(world) {}

        void OnUpdate(double deltaTime) override
        {
            for(Entity e : m_World->GetGroup<TransformComponent, PhysicsBodyComponent>())
            {
                TransformComponent& transform = m_World->GetComponent<TransformComponent>(e);
                PhysicsBodyComponent& body = m_World->GetComponent<PhysicsBodyComponent>(e);
                glm::vec3 prevPosition = transform.Position;
                glm::vec3 newPosition = transform.Position + body.Velocity + body.Acceleration * ((float)deltaTime * (float)deltaTime) * AccelerationMultiplier;

                glm::vec3 dragForce = 0.5f * body.Drag * (body.Velocity * abs(body.Velocity));
                glm::vec3 dragAccelertion = dragForce / body.Mass;
                glm::vec3 appliedAcceleration = body.Force / body.Mass;

                glm::vec3 newAcceleration = appliedAcceleration + body.Gravity;// - dragAccelertion;

                transform.Position = newPosition;
                body.Acceleration = newAcceleration;
                body.Velocity = newPosition - prevPosition;
            }
        }
    };
    float PhysicsSystem::AccelerationMultiplier = 1.0f;
}


