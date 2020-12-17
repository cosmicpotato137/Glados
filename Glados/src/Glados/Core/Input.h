#pragma once

#include "Core.h"

namespace Glados {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int keycode);
		static int GetMouseScroll();
		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};

}
