#pragma once

#ifdef OLD_PLATFORM_WINDOWS
	#ifdef OLD_BUILD_DLL
		#define OLD_API __declspec(dllexport)
	#else
		#define OLD_API __declspec(dllimport)
	#endif
#else
	#error Overlord only support Windows
#endif

#define BIT(x) (1 << x)