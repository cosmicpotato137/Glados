#pragma once

namespace Glados {

	class Texture 
	{
	public:
		~Texture() = default;

		// many different texture slots in gpus modern gpu has abt 8
		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual inline int GetWidth() const = 0;
		virtual inline int GetHeight() const = 0;

		static Ref<Texture> Create(const std::string& filepath);
	};

}