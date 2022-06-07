#include "Clap.h"

#include "System.h"
#include "Components.h"
#include "PostProcess.h"
#include "RenderSystem.h"

using namespace Clap;

namespace Game2D
{
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
                    auto texture = m_World->GetComponent<SpriteComponent>(cameraController.Target).Texture;
                    glm::vec3 offset = glm::vec3(cameraController.Offset.x * texture->GetWidth(), cameraController.Offset.y * texture->GetWidth(), 0.0f);
                    transform.Position = glm::mix(transform.Position, target.Position + offset, cameraController.FollowVelocity);
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
            for(Entity e : m_World->GetGroup<TransformComponent, PlayerControllerComponent, RPGPhysicsBodyComponent>())
            {
                TransformComponent& transform = m_World->GetComponent<TransformComponent>(e);
                RPGPhysicsBodyComponent& body = m_World->GetComponent<RPGPhysicsBodyComponent>(e);
                PlayerControllerComponent& player = m_World->GetComponent<PlayerControllerComponent>(e);
                int inputX = (int)InputX.x - (int)InputX.y;
                int inputY = (int)InputY.y - (int)InputY.x;
                if(inputX != 0 || inputY != 0)
                {
                    glm::vec2 direction = glm::normalize(glm::vec2(inputX, inputY));
                    body.Velocity = glm::vec3(direction, 0.0f) * player.Speed;
                }
                else body.Velocity = glm::vec3(0.0f); //EVENTS NOT WORKINGGGG
            }
        }
        void OnEvent(Event& e)
        {
            switch(e.Type)
            {
                case EventType::KeyPressed:
                    switch(e.Data.KeyEvent.KeyCode)
                    {
                        case KEY_W:
                            InputY.x = 1.0f;
                        break;
                        case KEY_S:
                            InputY.y = 1.0f;
                        break;
                        case KEY_D:
                            InputX.x = 1.0f;
                        break;
                        case KEY_A:
                            InputX.y = 1.0f;
                        break;
                    }
                break;
                case EventType::KeyReleased:
                    switch(e.Data.KeyEvent.KeyCode)
                    {
                        case KEY_W:
                            InputY.x = 0.0f;
                        break;
                        case KEY_S:
                            InputY.y = 0.0f;
                        break;
                        case KEY_D:
                            InputX.x = 0.0f;
                        break;
                        case KEY_A:
                            InputX.y = 0.0f;
                        break;
                    }
                break;
                default: break;
            }
        }

    private:
        glm::vec2 InputX = {0.0f, 0.0f};
        glm::vec2 InputY = {0.0f, 0.0f};
    };

    class RPGPhysicsSystem : public System
    {
    public:
        RPGPhysicsSystem(Ref<World> world) : System(world) {}

        void OnUpdate(double deltaTime) override
        {
            for(Entity e : m_World->GetGroup<TransformComponent, RPGPhysicsBodyComponent>())
            {
                TransformComponent& transform = m_World->GetComponent<TransformComponent>(e);
                RPGPhysicsBodyComponent& body = m_World->GetComponent<RPGPhysicsBodyComponent>(e);
                
                transform.Position = transform.Position + body.Velocity * (float)deltaTime * 60.0f/*Accounting for deltatime*/;
            }
        }
    };

    class PhysicsSystem : public System
    {
    public:
        PhysicsSystem(Ref<World> world) : System(world) {}

        void OnUpdate(double deltaTime) override
        {
            for(Entity e : m_World->GetGroup<TransformComponent, PhysicsBodyComponent>())
            {
                TransformComponent& transform = m_World->GetComponent<TransformComponent>(e);
                PhysicsBodyComponent& body = m_World->GetComponent<PhysicsBodyComponent>(e);
                glm::vec3 prevPosition = transform.Position;
                glm::vec3 newPosition = transform.Position + body.Velocity + body.Acceleration * ((float)deltaTime * (float)deltaTime);

                glm::vec3 dragForce = 0.5f * body.Drag * (body.Velocity * abs(body.Velocity));
                glm::vec3 dragAccelertion = dragForce / body.Mass;
                glm::vec3 appliedAcceleration = body.Force / body.Mass;

                glm::vec3 newAcceleration = appliedAcceleration + body.Gravity;// - dragAccelertion;
                //TODO: FIGURE THE PHYSICS OUT, DELTATIME WAS WRONG, NOW IT IS CORRECT, HAVEN'T TESTED

                transform.Position = newPosition;
                body.Acceleration = newAcceleration;
                body.Velocity = newPosition - prevPosition;
            }
        }
    };
}


