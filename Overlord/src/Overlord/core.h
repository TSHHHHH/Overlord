#pragma once

#ifdef OLD_PLATFORM_WINDOWS
	#ifdef OLD_BUILD_DLL
		#define OLD_API __declspec(dllexport)
	#else
		#define OLD_APT __declspec(dllimport)
	#endif
#else
	#error Overlord only support Windows
#endif // OLD_PLATFORM_WINDOWS
