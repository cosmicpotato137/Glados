#pragma once

#include "Core/Core.h"
#include <string>

namespace Glados {

	class GLADOS_API Texture
	{
	private:
		unsigned int m_RendererID;
		std::string m_Filepath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	public:
		Texture(const std::string& filepath);
		~Texture();

		// many different texture slots in gpus modern gpu has abt 8
		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
	};

}