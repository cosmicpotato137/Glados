#include "Glados.h"

using namespace Glados;

class SandboxLayer : public Glados::Layer
{
public:
	SandboxLayer()
		: Layer("Example")
	{
	}

	void OnUpdate()
	{
	}

	void OnEvent(Glados::Event& e) override
	{
		GD_TRACE("{0}", e);
	}
};

class Sandbox : public Glados::Application
{
public:
	Sandbox()
		: Application()
	{
		PushLayer(new SandboxLayer());

		PushOverlay(new ImGuiLayer());
	}

	~Sandbox()
	{
	}
};

Glados::Application* Glados::CreateApplication()
{
	return new Sandbox();
}