#include "Glados.h"
#include "Glados/Core/EntryPoint.h"
#include "TestLayer.h"

class Sandbox : public Glados::Application
{
public:
	Sandbox()
		: Application()
	{
		PushLayer(new SandboxLayer());

		//PushOverlay(new ImGuiLayer());
	}

	~Sandbox()
	{
	}
};

Glados::Application* Glados::CreateApplication()
{
	return new Sandbox();
}