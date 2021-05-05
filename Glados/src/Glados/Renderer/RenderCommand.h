#pragma once
#include "RendererAPI.h"

namespace Glados {

	class RenderCommand
	{
	private:
		static Scope<RendererAPI> s_RendererAPI;

	public:
		static std::string GetVersion() { return s_RendererAPI->GetVersion(); }
		static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }

		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void SetBlend(bool blend);

		static void Clear();
		static void DrawIndexed(const Ref<VertexArray> vertexArray, uint32_t count = 0);
	};

}