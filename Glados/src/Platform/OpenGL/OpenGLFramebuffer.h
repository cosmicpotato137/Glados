#pragma once
#include "Glados/Renderer/Framebuffer.h"

namespace Glados {

	class OpenGLFramebuffer : public Framebuffer
	{
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;

	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(int width, int height);

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	};

}