#include "Glados.h"
#include "Glados/Core/EntryPoint.h"
#include "TestLayer.h"

// TEMPORARY
#include "glad/glad.h"

class Sandbox : public Glados::Application
{
public:
	Sandbox()
		: Application()
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