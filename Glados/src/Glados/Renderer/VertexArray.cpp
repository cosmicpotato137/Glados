#include "gladospch.h"

#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Glados {

	Ref<Glados::VertexArray> VertexArray::Create()
	{
		return CreateRef<OpenGLVertexArray>();
	}

}