#include "gladospch.h"
#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Glados {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		GD_CORE_ASSERT(windowHandle, "windowHandle is null!");
	}

	void OpenGLContext::Init()
	{
		// init gl library using glad
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GD_CORE_ASSERT(status, "Failed to initialize Glad!");

		// assert opengl version
		bool isCorrectVersion = (GLVersion.major >= 4 && GLVersion.minor >= 3) || (GLVersion.major >= 5);
		GD_CORE_ASSERT(isCorrectVersion, "Glados only supports OpenGL 4.3 and later!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}