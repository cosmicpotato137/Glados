#include "Glados.h"
#include "Glados/Core/EntryPoint.h"
#include "TestLayer.h"

class EmptyLayer : public Glados::Layer
{
public:
	EmptyLayer() : Layer("Empty Layer") {}
	~EmptyLayer() {}

	void OnEvent(Event& e) 
	{
		GD_INFO("Event: {0}", e.GetName());
	}
};

class Sandbox : public Glados::Application
{
public:
	Sandbox()
		: Application()
	{
		PushLayer(new TestLayer());
		PushLayer(new EmptyLayer());
	}

	~Sandbox()
	{
	}
};

Glados::Application* Glados::CreateApplication()
{
	return new Sandbox();
}