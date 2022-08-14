#pragma once

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
        glm::vec4 Data; //{TexIndex; TilingFactor; Radius; OutlineThickness}
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

        //General purpose rendering
        static void Submit(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& uv, const glm::vec4& color);
        static void Submit(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec2& from, const glm::vec2& to, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        static void Submit(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float radius = 0.0f, float outlineRadius = 0.0f);
        static void Submit(const glm::mat4& transform, const glm::vec4& color);
        static void Submit(const glm::mat4& transform, Ref<Texture2D>& texture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, bool scaleByTexture = true, const glm::vec4& uv = {0.0f, 0.0f, 1.0f, 1.0f});
        static void Submit(const glm::mat4& transform, Ref<Texture2D>& texture, const glm::vec2& from, const glm::vec2& to, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, bool scaleByTexture = true);
        static void Submit(const glm::mat4& transform, Ref<Texture2D>& texture, glm::vec3 positions[4], glm::vec2 uv[4], const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});

        static void SubmitTile(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec2& tileSize, uint32_t tile, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});

        //UI Rendering, Requires a separate shader to handle outlines and such
        //static void Submit(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float radius = 0.0f, float outlineRadius = 0.0f);
        //TODO: FIGURE OUT THE UV SITUATION

        static void SetShader(Ref<Shader> shader);

        static void Shutdown();
    };  
}