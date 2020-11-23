#pragma once

#include "Core/Core.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Glados {

    class Renderer
    {
    public:
        static void Init();
        void Clear() const;
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    };

}