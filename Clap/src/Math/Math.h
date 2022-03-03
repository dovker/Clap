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

        glm::mat4 GetTransform() const;
        Transform& operator +=(const Transform& t2);

        friend Transform operator +(const Transform& t1, const Transform& t2);
    };
}