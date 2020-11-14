#pragma once
#include "gladospch.h"

#include "Core.h"

class GLFWwindow;

namespace Glados {

	class GLADOS_API Application
	{
	protected:
		GLFWwindow* window;
	public:
		Application(unsigned int width, unsigned int height, const std::string& title);
		virtual ~Application();

		void Run();
	};

	// to be defined in client
	Application* CreateApplication();

}