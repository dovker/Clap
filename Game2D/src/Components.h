#pragma once
#include "Clap.h"

using namespace Clap;

namespace Game2D
{
    using TransformComponent = Transform;
    struct CameraComponent
    {
        glm::mat4 Projection;
        bool Primary = true;
    };
    struct CameraControllerComponent
    {
        Entity Target = NULL_ENTITY;
        float FollowVelocity = 0.1f;
        glm::vec2 Offset;
    };
    struct PlayerControllerComponent
    {
        glm::vec3 Acceleration;
    };
    struct PhysicsBodyComponent
    {
        glm::vec3 Acceleration = {0.0f, 0.0f, 0.0f};
        glm::vec3 Velocity = {0.0f, 0.0f, 0.0f};
        glm::vec3 Force = {0.0f, 0.0f, 0.0f};
        glm::vec3 Gravity = {0.0f, 0.0f, 0.0f};
        float Mass = 1.0f;
        float Drag = 0.1f;
    };
    struct SpriteComponent
    {
        Ref<Texture2D> Texture;
        bool ScaleToTexture = true;

        glm::vec2 Pivot = {0.0f, 0.0f};
        bool FlippedX = false;
        bool FlippedY = false;
        SpriteComponent() {};
        SpriteComponent(Ref<Texture2D> tex) : Texture(tex) {};
    };
    struct SquareColliderComponent
    {
        glm::vec2 Size;
        glm::vec2 Offset;
        bool BasedOnSprite;
    };

}