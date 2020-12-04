#include "gladospch.h"
#include "VertexArray.h"

namespace Glados {

    VertexArray::VertexArray()
    {
        GLCall(glGenVertexArrays(1, &m_RendererID));
    }

    VertexArray::~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &m_RendererID));
    }

    void VertexArray::AddBuffer(const VertexBuffer& vb, unsigned int lpos)
    {
        Bind();
        vb.Bind();
        // list of all elements in m_Elements vector
        BufferLayout layout = vb.GetLayout();
        for (BufferElement e : layout)
        {
            const auto& element = elements[i];
            // pushes data to the target buffer
            GLCall(glEnableVertexAttribArray(e.));
            // formatting of data in current 
            GLCall(glVertexAttribPointer(i + lpos, element.count, element.type,
                element.normalized, layout.ByteSize(), (const void*)element.offset));
        }
    }

    void VertexArray::Bind() const
    {
        GLCall(glBindVertexArray(m_RendererID));
    }

    void VertexArray::Unbind() const
    {
        GLCall(glBindVertexArray(0));
    }

}