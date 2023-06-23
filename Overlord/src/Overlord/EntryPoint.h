#pragma once

#ifdef OLD_PLATFORM_WINDOWS

extern Overlord::Application* Overlord::CreateApplication();

int main()
{
	// Init logging system
	Overlord::Log::Init();
	OLD_CORE_WARN("Initialized Log!");

	// Init application
	auto app = Overlord::CreateApplication();

	app->Run();

	delete app;
}

#else
	#error Overlord only support Windows
#endif // OLD_PLATFORM_WINDOWS
