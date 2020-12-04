#pragma once
#include "RendererAPI.h"
#include "glm/glm.hpp"
#include "VertexArray.h"
#include "Shader.h"

using namespace glm;

namespace Glados {

	class Renderer
	{
	private:
		static Scope<RendererAPI> s_RendererAPI;
	public:
		Renderer() = delete;
		~Renderer() = delete;

		static void SetAPI(RendererAPI::API api);
		static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }
		static std::string GetAPIVersion() { return s_RendererAPI->GetVersion(); }

		static void Init();
		static void Shutdown();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const vec4& color);
		static void Blend(bool blend);

		static void Clear();
		static void DrawIndexed(const Ref<VertexArray> vertexArray, uint32_t count = 0);

	};

}