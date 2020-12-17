#include "gladospch.h"
#include "Glados/Core/Input.h"
#include "Glados/Core/GDApplication.h"
#include "GLFW/glfw3.h"

namespace Glados {

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, keycode);
		return state == GLFW_PRESS;
	}

	int Input::GetMouseScroll()
	{
		return 0;
	}

	std::pair<float, float> Input::GetMousePos()
	{
		double xpos, ypos;
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwGetCursorPos(window, &xpos, &ypos);
		return std::pair<float, float>((float)xpos, (float)ypos);
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePos();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePos();
		return y;
	}

}