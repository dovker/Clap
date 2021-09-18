#pragma once


// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define CLAP_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define CLAP_PLATFORM_IOS
	#elif TARGET_OS_MAC == 1
		#define CLAP_PLATFORM_MACOS
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define CLAP_PLATFORM_ANDROID
#elif defined(__linux__)
	#define CLAP_PLATFORM_LINUX
#else
	#error "Unknown platform!"
#endif // End of platform detection



#define CLAP_ENABLE_ASSERTS

#ifdef CLAP_ENABLE_ASSERTS
    #ifdef CLAP_PLATFORM_WINDOWS
        #define CLAP_ASSERT(x, ...) { if(!(x)) { CLAP_ERROR("Assertion Failed: ", __VA_ARGS__); __debugbreak(); } }
    #else
        #define CLAP_ASSERT(x, ...) { if(!(x)) { CLAP_ERROR("Assertion Failed: ", __VA_ARGS__); } }
    #endif
#else
#define CLAP_ASSERT(x, ...)
#endif


//Forwards a function as a lambda
#define CLAP_FORWARD_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "pch.h"

namespace Clap
{
	//Scoped Pointer
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

	//This engine will be using std::shared_ptr for reference counting.
	//Garbage collected pointer
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}