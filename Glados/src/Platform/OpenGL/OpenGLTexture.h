#pragma once
#include "Glados/Renderer/Texture.h"
#include <string>

namespace Glados {

	class OpenGLTexture : public Texture
	{
	private:
		uint32_t m_RendererID;
		std::string m_Filepath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	public:
		OpenGLTexture(const std::string& filepath);
		~OpenGLTexture();

		// many different texture slots in gpus modern gpu has abt 8
		virtual void Bind(unsigned int slot = 0) const override;
		virtual void Unbind() const override;

		virtual inline int GetWidth() const override { return m_Width; }
		virtual inline int GetHeight() const override { return m_Height; }
	};

}