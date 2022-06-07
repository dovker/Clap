#include <glm/glm.hpp>

#include "Clap.h"

using namespace Clap;

namespace Game
{
    class Renderer
    {

        static void Init();
        static void Begin(const glm::mat4& projection, const glm::mat4& transform);


        static void Submit(Ref<Mesh> mesh, const Transform& transform);

        static void SubmitLight();

        static void SetShader(Ref<Shader> shader);

        static void Shutdown();
    };  
}