#include "Math.h"

namespace Clap
{
    glm::mat4 Transform::GetTransform() const
    {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

        return glm::translate(glm::mat4(1.0f), Position)
            * rotation
            * glm::scale(glm::mat4(1.0f), Scale);
    }

    Transform& Transform::operator +=(const Transform& t2)
    {
        this->Position += t2.Position;
        this->Rotation += t2.Rotation;
        this->Scale *= t2.Scale;
        return *this;
    }

    Transform operator +(const Transform& t1,  const Transform& t2)
    {
        Transform t = t1;
        t.Position += t2.Position;
        t.Rotation += t2.Rotation;
        t.Scale *= t2.Scale;
        return t;
    }
}