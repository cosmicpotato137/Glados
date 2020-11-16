#pragma once

#ifdef GD_PLATFORM_WINDOWS

extern Glados::Application* Glados::CreateApplication();

int main(int argc, int** argv)
{
	Glados::Log::Init();
	GD_CORE_TRACE("Initialized Core Logger");
	GD_CORE_INFO("Welcome to Glados!");

	auto app = Glados::CreateApplication();
	app->Run();
	delete app;
}

#endif