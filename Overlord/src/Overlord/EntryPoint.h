#pragma once

#ifdef OLD_PLATFORM_WINDOWS

extern Overlord::Application* Overlord::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Overlord::CreateApplication();
	app->Run();
	delete app;
}

#else
	#error Overlord only support Windows
#endif // HZ_PLATFORM_WINDOWS
