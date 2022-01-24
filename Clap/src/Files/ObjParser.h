#pragma once

#include <pch.h>
#include "Graphics/Mesh.h"

namespace Clap
{
    class ObjParser
    {
    public:
        static Ref<Mesh> Parse(const std::string& filename, bool precalculateTangents = true);
        static Ref<Mesh> ParseString(const std::string& string);
    };
}