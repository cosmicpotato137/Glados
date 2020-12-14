#pragma once
#include "Glados/Renderer/Framebuffer.h"

namespace Glados {

	class OpenGLFramebuffer : public Framebuffer
	{
	private:
		uint32_t m_RendererID;
		FramebufferSpecification m_Specification;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;

	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	};

}