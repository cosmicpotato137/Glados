#include "Glados.h"

class SandboxLayer : public Glados::Layer
{
public:
	SandboxLayer()
		: Layer("Example")
	{
	}

	void OnUpdate()
	{
		GD_INFO("Example Layer Update");
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
	{
		PushLayer(new SandboxLayer());
	}

	~Sandbox()
	{
	}
};

Glados::Application* Glados::CreateApplication()
{
	return new Sandbox();
}