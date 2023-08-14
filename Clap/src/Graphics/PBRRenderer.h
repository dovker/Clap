#pragma once

#include "Core/Core.h"
#include "Math/Math.h"
#include "API/Shader.h"
#include "API/Framebuffer.h"
#include "Model.h"

#include <glm/glm.hpp>

namespace Clap
{
    struct PostProcessingUniform
    {
        float Exposure = 1.0f;        
        float WhiteBallance = 0.0f;
        float Tint = 0.0f;
        float Contrast = 0.0f;
        float Brightness = 0.0f;
        float Saturation = 1.0f;
        float Gamma = 2.2f;

    };

    class PBRRenderer
    {
    public:

        static void Init(uint32_t width, uint32_t height);
        static void Begin(const glm::mat4& projection, const glm::mat4& view);
        static void Submit(const Ref<Model>& model, Transform transform);
        static void End();

    };
}