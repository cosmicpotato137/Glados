#include "gladospch.h"

#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Glados {

	void APIENTRY OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         GD_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       GD_CORE_ERROR(message);	return;
		case GL_DEBUG_SEVERITY_LOW:          GD_CORE_WARN(message);		return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: GD_CORE_TRACE(message);	return;
		}
		GD_CORE_ASSERT(false, "Unknown severity level!");
	}

	OpenGLRendererAPI::OpenGLRendererAPI()
	{
		SetClearColor(vec4(0, 0, 0, 0));
	}

	OpenGLRendererAPI::~OpenGLRendererAPI()
	{

	}

	void OpenGLRendererAPI::Init()
	{
#ifdef GD_DEBUG
		GD_CORE_TRACE("Initiating OpenGL debug funciton");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, 0);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, 
			GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

		glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER,
			GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_FALSE);
#endif
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Blend(bool blend)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (blend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		// TODO: delete comments
		vertexArray->Bind();
		uint32_t c = count ? count : vertexArray->GetIndexBuffer().GetCount();
		glDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, nullptr);
	}

	std::string OpenGLRendererAPI::GetVersion()
	{
		std::string version((const char*)glGetString(GL_VERSION));
		return version;
	}

}