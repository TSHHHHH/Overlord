#pragma once

#include <memory>

#ifdef OLD_PLATFORM_WINDOWS
	#if OLD_DYNAMIC_LINK
		#ifdef OLD_BUILD_DLL
			#define OLD_API __declspec(dllexport)
		#else
			#define OLD_API __declspec(dllimport)
		#endif
	#else
		#define OLD_API
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
#endif

#define BIT(x) (1 << x)

#define OLD_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Overlord
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}