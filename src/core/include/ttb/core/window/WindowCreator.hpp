#if defined( PLATFORM_LINUX )
#include "platform_linux/WindowCreator.hpp"
#elif defined( PLATFORM_ANDROID )
#include "platform_android/WindowCreator.hpp"
#else
#error "Platform not supported yet"
#endif
