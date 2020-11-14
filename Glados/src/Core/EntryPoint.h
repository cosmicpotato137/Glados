#pragma once

#ifdef GD_PLATFORM_WINDOWS

extern Glados::Application* Glados::CreateApplication();

int main(int argc, int** argv)
{
	//Hazel::Log::Init();
	//HZ_CORE_TRACE("Initialized Core Logger");
	//HZ_CORE_WARN("Warining test!");
	//HZ_INFO("Hello! Var={0}", 5);

	auto app = Glados::CreateApplication();
	app->Run();
	delete app;
}

#endif