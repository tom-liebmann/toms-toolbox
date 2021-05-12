#include <ttb/core/Logger.hpp>

#include <iostream>
#include <memory>
#include <unordered_map>


namespace ttb
{
    Logger& Logger::instance( char loglevel )
    {
        static std::unordered_map< char, std::unique_ptr< Logger > > map;

        if( auto iter = map.find( loglevel ); iter != std::end( map ) )
        {
            return *iter->second;
        }
        else
        {
            auto iter2 = map.emplace( loglevel, new Logger{ loglevel } );

            if( iter2.second )
            {
                return *iter2.first->second;
            }
            else
            {
                throw std::runtime_error( "Unable to create logger" );
            }
        }
    }

    Logger::Logger( char loglevel ) : std::ostream( &m_buffer ), m_buffer{ loglevel }
    {
    }


    Logger& logger( char loglevel )
    {
        return Logger::instance( loglevel );
    }


    Logger::Buffer::Buffer( char loglevel ) : m_loglevel{ loglevel }
    {
    }

    Logger::Buffer::~Buffer()
    {
        if( pbase() != pptr() )
        {
            flush();
        }
    }

    int Logger::Buffer::sync()
    {
        flush();
        return 0;
    }

#if defined( PLATFORM_LINUX ) || defined( PLATFORM_BROWSER )

    void Logger::Buffer::flush()
    {
        std::cout << m_loglevel << ": " << str();
        str( "" );
        std::cout.flush();
    }

#elif defined( PLATFORM_ANDROID )

#include <android/log.h>

    void Logger::Buffer::flush()
    {
        switch( m_loglevel )
        {
            case 'I':
                __android_log_print(
                    ANDROID_LOG_INFO, "com.juicy_trash.pong_royale", "%s", str().c_str() );
                break;

            case 'D':
                __android_log_print(
                    ANDROID_LOG_DEBUG, "com.juicy_trash.pong_royale", "%s", str().c_str() );
                break;

            case 'E':
            default:
                __android_log_print(
                    ANDROID_LOG_ERROR, "com.juicy_trash.pong_royale", "%s", str().c_str() );
                break;
        }

        str( "" );
    }

#endif
}
