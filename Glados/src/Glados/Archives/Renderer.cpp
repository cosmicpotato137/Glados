#include "gladospch.h"

#include "Renderer.h"

namespace Glados {

    void Renderer::Init()
    {
    }

    void Renderer::Clear() const
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
    {
        shader.Bind();
        va.Bind();
        ib.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.GetNumElements(), GL_UNSIGNED_INT, nullptr));
    }

}