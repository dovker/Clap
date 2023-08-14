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

    bool within(float min, float x, float max)
    {
        return min <= x && x <= max;
    }

    bool AABB::Collides(const glm::mat4& matrix)
    {
        glm::vec4 corners[8] = {
            {Min.x, Min.y, Min.z, 1.0},
            {Max.x, Min.y, Min.z, 1.0},
            {Min.x, Max.y, Min.z, 1.0},
            {Max.x, Max.y, Min.z, 1.0},

            {Min.x, Min.y, Max.z, 1.0},
            {Max.x, Min.y, Max.z, 1.0},
            {Min.x, Max.y, Max.z, 1.0},
            {Max.x, Max.y, Max.z, 1.0},
        };

        bool inside = false;

        for (size_t corner_idx = 0; corner_idx < 8; corner_idx++) {
            // Transform vertex
            glm::vec4 corner = matrix * corners[corner_idx];
            // Check vertex against clip space bounds
            inside = inside ||
                within(-corner.w, corner.x, corner.w) &&
                within(-corner.w, corner.y, corner.w) &&
                within(0.0f, corner.z, corner.w);
        }
        return inside;
    }
}