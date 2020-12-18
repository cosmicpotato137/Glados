#pragma once
//#include "Glados/Core/Core.h"

namespace Glados {

	struct FramebufferSpecification
	{
		int Width, Height;
		// FramebufferFormat Format = ;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(int width, int height) = 0;
		
		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};


}