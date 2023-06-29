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

#ifdef OLD_DEBUG
	#define OLD_ENABLE_ASSERTS
#endif

#ifdef  OLD_ENABLE_ASSERTS
	#define OLD_ASSERT(x, ...) { if (!(x)) { OLD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define OLD_CORE_ASSERT(x, ...) { if (!(x)) { OLD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define OLD_ASSERT(x, ...)
	#define OLD_CORE_ASSERT(x, ...)
#endif //  OLD_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define OLD_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)