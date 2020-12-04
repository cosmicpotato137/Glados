#pragma once
#include "Glados/Renderer/RendererAPI.h"
#include "glm/glm.hpp"

using namespace glm;

namespace Glados {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();
		~OpenGLRendererAPI();

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const vec4& color) override;
		virtual void Blend(bool blend) override;
		
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& VertexArray, uint32_t count) override;

		virtual std::string GetVersion() override;
	};

}