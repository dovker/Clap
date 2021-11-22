#pragma once

#include <glm/glm.hpp>

#include "Core/Core.h"
#include "Types.h"

#ifndef CLAP_ANISOTROPY_AMOUNT
    #define CLAP_ANISOTROPY_AMOUNT 8.0f
#endif

#include "VertexArray.h"

namespace Clap
{
    class Graphics
    {
        static void Init();
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static void SetClearColor(const glm::vec4& color);
        static void SetBlendingMode(BlendingMode mode);
        static void Clear();
        
        static void ToggleDepthTest(bool toggle);
        static void ToggleBlending(bool toggle);
        static void ToggleBackfaceCulling(bool toggle);

        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);
    };

}
