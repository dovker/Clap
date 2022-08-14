#pragma once
#include "pch.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Clap
{
    struct Transform
    {
        glm::vec3 Position, Rotation, Scale;
        //Transform() = default;
        Transform(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
            : Position(pos), Rotation(rot), Scale(scale)
        {}

        Transform(const Transform& transform)
            : Position(transform.Position), Rotation(transform.Rotation), Scale(transform.Scale)
        {}

        glm::mat4 GetTransform() const;
        Transform& operator +=(const Transform& t2);

        friend Transform operator +(const Transform& t1, const Transform& t2);
    };

    struct AABB
    {
        glm::vec2 Position, Size, Origin;

        AABB(glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size = glm::vec2(0.0f), glm::vec2 origin = glm::vec2(0.0f))
            : Position(pos), Size(size), Origin(origin)
        {}

        glm::vec2 RealPosition() const
        {
            return Position - Size * Origin;
        }

        float Left() const
        {
            return Position.x - Size.x * Origin.x;
        }

        float Right() const
        {
            return Position.x + Size.x * (1.0f - Origin.x);
        }

        float Bottom() const
        {
            return Position.y - Size.y * Origin.y;
        }

        float Top() const
        {
            return Position.y + Size.y * (1.0f - Origin.y);
        }

        bool Collides(const glm::vec2& point) const
        {
            return Left()   < point.x &&
                   Right()  > point.x &&
                   Bottom() < point.y &&
                   Top()    > point.y; 
        }

        bool Collides(const AABB& parent, const glm::vec2& point) const
        {
            return parent.RealPosition().x + Left()   < point.x &&
                   parent.RealPosition().x + Right()  > point.x &&
                   parent.RealPosition().y + Bottom() < point.y &&
                   parent.RealPosition().y + Top()    > point.y; 
        }

        bool Collides(const AABB& other) const
        {
            return Left()   < other.Right() &&
                   Right()  > other.Left()  &&
                   Bottom() < other.Top()   &&
                   Top()    > other.Bottom(); 
        }
    };
}