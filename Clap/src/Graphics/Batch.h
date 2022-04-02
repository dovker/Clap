#include "Graphics/API/Texture.h"
#include "Graphics/API/Shader.h"
#include "Math/Math.h"
#include <glm/glm.hpp>

namespace Clap
{
    struct QuadVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        float TexIndex;
    };

    class Batch
    {
    public:
        static void Init();
        static void Begin(const glm::mat4& projection, const glm::mat4& view);

        static void BeginBatch();
        static void End();
        static void Flush();
        static void FlushAndReset();

        static void Submit(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& uv, const glm::vec4& color);
        static void Submit(const glm::mat4& transform, const glm::vec4& color);
        static void Submit(const glm::mat4& transform, Ref<Texture2D>& texture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, bool scaleByTexture = true, const glm::vec4& uv = {0.0f, 0.0f, 1.0f, 1.0f});

        static void SetShader(Ref<Shader> shader);

        static void Shutdown();
    };  
}