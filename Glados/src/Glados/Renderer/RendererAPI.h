#pragma once
#include "VertexArray.h"
#include "glm/glm.hpp"

using namespace glm;

namespace Glados {

	enum BlendMode { DEFAULT = 0, ADD, ALPHA };
	
	class RendererAPI
	{
	public:
		enum class API { None = 0, OpenGL };
	private:
		static API s_API;
	public:
		~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const vec4& color) = 0;
		virtual void Blend(BlendMode mode) = 0;
		virtual void SetFaceCull(bool mode) = 0;
		
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count) = 0;
		
		virtual std::string GetVersion() = 0;

		static API GetAPI() { return s_API; }

		static Scope<RendererAPI> Create();
	};

}