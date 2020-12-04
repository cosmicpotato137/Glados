#include "gladospch.h"
#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Glados {

	Glados::Ref<Texture> Texture::Create(const std::string& filepath)
	{
		return CreateRef<OpenGLTexture>(filepath);
	}

}