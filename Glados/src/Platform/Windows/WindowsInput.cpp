#include "gladospch.h"
#include "WindowsInput.h"
#include "Glados/Core/GDApplication.h"
#include "GLFW/glfw3.h"

namespace Glados {

	Input* Input::s_Instance = new WindowsInput();

	bool Glados::WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

}