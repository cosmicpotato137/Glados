#include "Glados.h"

class Sandbox : public Glados::Application
{
public:
	Sandbox()
		: Application(960, 540, "Sandbox")
	{
	}

	~Sandbox()
	{
	}
};

Glados::Application* Glados::CreateApplication()
{
	return new Sandbox();
}