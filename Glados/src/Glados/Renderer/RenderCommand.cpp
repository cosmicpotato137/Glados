#include "gladospch.h"
#include "RenderCommand.h"

namespace Glados {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = nullptr;

	void RenderCommand::Init()
	{
		s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Init();
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::Clear()
	{
		s_RendererAPI->Clear();
	}

	void RenderCommand::SetClearColor(const vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	void RenderCommand::SetBlend(bool blend)
	{
		s_RendererAPI->Blend(blend);
	}

	void RenderCommand::DrawIndexed(const Ref<VertexArray> vertexArray, uint32_t count)
	{
		s_RendererAPI->DrawIndexed(vertexArray, count);
	}

}