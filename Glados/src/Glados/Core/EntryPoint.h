#pragma once

#ifdef GD_PLATFORM_WINDOWS

extern Glados::Application* Glados::CreateApplication();

int main(int argc, int** argv)
{
	Glados::Log::Init();
	GD_CORE_TRACE("Initialized Core Logger");
	GD_CORE_INFO("Welcome to Glados!");
	GD_CORE_WARN("Warining test!");
	GD_INFO("Hello! Var={0}", 5);

	auto app = Glados::CreateApplication();
	app->Run();
	delete app;
}

#endif