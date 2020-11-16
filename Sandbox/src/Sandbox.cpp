#include "Glados.h"

class Sandbox : public Glados::Application
{
public:
	Sandbox()
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