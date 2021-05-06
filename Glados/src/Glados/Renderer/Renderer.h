#pragma once
#include "RenderCommand.h"
#include "glm/glm.hpp"
#include "VertexArray.h"
#include "Shader.h"
#include "Framebuffer.h"

using namespace glm;

namespace Glados {

	class Renderer
	{
	private:
		static Scope<ShaderLibrary> s_ShaderLib;
		static Ref<Shader> s_DefaultShader;
	public:
		Renderer() = delete;
		~Renderer() = delete;

		static RendererAPI::API GetRenderAPI() { return RenderCommand::GetAPI(); }
		static ShaderLibrary& GetShaderLibrary() { return *s_ShaderLib; }
		static std::string GetRendererAPIVersion() { return RenderCommand::GetVersion(); }

		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform);

		static void SetDefaultShader(const Ref<Shader>& defaultShader) { s_DefaultShader = defaultShader; }
		static Ref<Shader> GetDefaultShader() { return s_DefaultShader; }
	};

}