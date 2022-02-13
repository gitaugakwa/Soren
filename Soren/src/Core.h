#pragma once

#ifdef SOREN_PLATFORM_WINDOWS
#if SOREN_DYNAMIC_LINK
	#ifdef SOREN_BUILD_DLL
		#define SOREN_API __declspec(dllexport)
	#else
		#define	SOREN_API __declspec(dllimport)
	#endif // SOREN_BUILD_DLL
#else
	#define SOREN_API
#endif

#else
	#error Soren only supports Windows!
#endif // SOREN_PLATFORM_WINDOWS

#ifdef SOREN_ENABLE_ASSERTS
	#define SOREN_ASSERT(x, ...) { if(!(x)) { SOREN_CORE_ERROR("Assertation Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define SOREN_CORE_ASSERT(x, ...) { if(!(x)) { SOREN_CORE_ERROR("Assertation Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SOREN_ASSERT(x, ...)
	#define SOREN_CORE_ASSERT(x, ...)
#endif // SOREN_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define SOREN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placehSORENders::_1)
