#include "gladospch.h"

#include "Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace Glados {

    void Renderer::Init()
    {
        /* Initialize the library */
        if (!glfwInit())
            ASSERT(false);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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