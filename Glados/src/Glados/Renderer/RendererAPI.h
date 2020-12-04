#pragma once
#include "VertexArray.h"
#include "glm/glm.hpp"

using namespace glm;
namespace Glados {

	using API = unsigned int;

	class RendererAPI
	{
	private:
		API s_API;
	public:
		~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const vec4& color) = 0;
		virtual void Blend(bool blend) = 0;
		
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count) = 0;
		
		virtual std::string GetVersion() = 0;

		API GetAPI() const { return s_API; }

		static Scope<RendererAPI> Create();
	};

}