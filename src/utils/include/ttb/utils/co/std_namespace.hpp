#pragma once

#if defined( __clang__ )
#include <experimental/coroutine>
namespace co = std::experimental;
#else
#include <coroutine>
namespace co = std;
#endif
