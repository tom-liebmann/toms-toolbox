#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <fmt/core.h>

#if defined( PLATFORM_ANDROID )
#include <android/log.h>
#define PACKAGE "com.juicy_trash.puzzle_game"
#endif


namespace ttb
{
    class Logger
    {
    public:
        Logger() = delete;

        template < typename... TArgs >
        static void info( fmt::format_string< TArgs... > fmt, TArgs&&... args );

        template < typename... TArgs >
        static void debug( fmt::format_string< TArgs... > fmt, TArgs&&... args );

        template < typename... TArgs >
        static void error( fmt::format_string< TArgs... > fmt, TArgs&&... args );
    };
}


namespace ttb
{
    template < typename... TArgs >
    void Logger::info( fmt::format_string< TArgs... > fmt, TArgs&&... args )
    {
#if defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )

        fmt::print( fmt, std::forward< TArgs... >( args )... );

#elif defined( PLATFORM_ANDROID )

        auto const text = fmt::format( fmt, std::forward< TArgs... >( args )... );
        __android_log_write( ANDROID_LOG_INFO, PACKAGE, text.c_str() );

#endif
    }

    template < typename... TArgs >
    void Logger::debug( fmt::format_string< TArgs... > fmt, TArgs&&... args )
    {
#if defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )

        fmt::print( fmt, std::forward< TArgs... >( args )... );

#elif defined( PLATFORM_ANDROID )

        auto const text = fmt::format( fmt, std::forward< TArgs... >( args )... );
        __android_log_write( ANDROID_LOG_DEBUG, PACKAGE, text.c_str() );

#endif
    }

    template < typename... TArgs >
    void Logger::error( fmt::format_string< TArgs... > fmt, TArgs&&... args )
    {
#if defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )

        fmt::print( fmt, std::forward< TArgs... >( args )... );

#elif defined( PLATFORM_ANDROID )

        auto const text = fmt::format( fmt, std::forward< TArgs... >( args )... );
        __android_log_write( ANDROID_LOG_ERROR, PACKAGE, text.c_str() );

#endif
    }
}
