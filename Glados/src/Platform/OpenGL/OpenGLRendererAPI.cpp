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
		case GL_DEBUG_SEVERITY_MEDIUM:       GD_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          GD_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: GD_CORE_TRACE(message); return;
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
		// get opengl version
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		GD_CORE_INFO("OpenGL Version: {0}.{1}", major, minor);

#ifdef GD_DEBUG
		GD_CORE_TRACE("Initiating OpenGL debug funciton");
		if ((major >= 4) && (minor >= 3))
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(
				GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
				0, NULL, GL_FALSE);
		}
		else
			GD_CORE_WARN("Unable to initiate OpenGL");
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::Shutdown()
	{
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const vec4& color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		IndexBuffer& indexBuffer = vertexArray->GetIndexBuffer();
		GD_CORE_ASSERT(&indexBuffer, "No IndexBuffer assigned to vertexArray");
		vertexArray->Bind();
		indexBuffer.Bind();
		uint32_t c = count ? count : vertexArray->GetIndexBuffer().GetCount();
		glDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, nullptr);
	}

	std::string OpenGLRendererAPI::GetVersion()
	{
		std::string version((const char*)glGetString(GL_VERSION));
		return version;
	}

}