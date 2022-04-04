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
        glm::vec2 Offset;
    };
    struct PlayerControllerComponent
    {
        glm::vec2 Acceleration;
        glm::vec2 Velocity;
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