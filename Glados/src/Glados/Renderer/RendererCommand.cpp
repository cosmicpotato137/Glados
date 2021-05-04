#include "gladospch.h"
#include "RendererCommand.h"

namespace Glados {

	Scope<RendererAPI> RendererCommand::s_RendererAPI = nullptr;

	void RendererCommand::Init()
	{
		s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Init();
	}

	void RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
	}

	void RendererCommand::Clear()
	{
		s_RendererAPI->Clear();
	}

	void RendererCommand::SetClearColor(const vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	void RendererCommand::SetBlend(bool blend)
	{
		s_RendererAPI->Blend(blend);
	}

	void RendererCommand::DrawIndexed(const Ref<VertexArray> vertexArray, uint32_t count)
	{
		s_RendererAPI->DrawIndexed(vertexArray, count);
	}

}