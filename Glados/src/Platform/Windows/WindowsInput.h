#pragma once
#include "Glados/Core/Input.h"

namespace Glados {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
	};

}
