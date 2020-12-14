#include "gladospch.h"
#include "Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Glados {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API: return CreateRef<OpenGLFramebuffer>(spec);
		}
	}

}

