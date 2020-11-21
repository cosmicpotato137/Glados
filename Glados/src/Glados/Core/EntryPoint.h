#pragma once

#ifdef GD_PLATFORM_WINDOWS

extern Glados::Application* Glados::CreateApplication();

int main(int argc, int** argv)
{
	Glados::Log::Init();

	auto app = Glados::CreateApplication();
	app->Run();
	delete app;
}

#endif