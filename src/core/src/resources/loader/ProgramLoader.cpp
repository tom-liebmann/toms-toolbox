#include <ttb/core/resources/loader/ProgramLoader.hpp>

#include <ttb/core/Logger.hpp>
#include <ttb/core/resources/io_helper.hpp>

#if defined( PLATFORM_ANDROID )
#include <android/log.h>
#include <ttb/core/resources/AssetManager.hpp>
#endif

#include <map>
#include <regex>
#include <sstream>


namespace ttb::resources
{
    namespace
    {
        auto const k_preProcCmd = std::regex( R"(^#\s*(\w+)(?:\s+(?:\"(.+)\"|(.+)))?)" );

        std::string loadSource( std::string const& rootPath, std::string const& filename );
    }
}


namespace ttb::resources
{
    ProgramLoader::ProgramLoader( std::string const& filename ) : DatabaseLoader( filename )
    {
    }

    std::shared_ptr< ttb::Program > ProgramLoader::load( YAML::Node const& node ) const
    {
        static std::map< std::string, ttb::Shader::Type > const shaderTypes = {
            { "fragment", ttb::Shader::Type::FRAGMENT },
            { "vertex", ttb::Shader::Type::VERTEX },
            { "geometry", ttb::Shader::Type::GEOMETRY },
        };

        return ttb::Program::create( [ & ]( auto& creator ) {
            for( auto const shaderNode : node )
            {
                auto const type = shaderTypes.at( shaderNode[ "type" ].as< std::string >() );
                auto const file = rootPath() + "/" + shaderNode[ "file" ].as< std::string >();

                auto const source = loadSource( rootPath(), file );

                try
                {
                    creator.attachShader( ttb::Shader::fromSource( type, source ) );
                }
                catch( std::runtime_error& e )
                {
                    throw std::runtime_error( file + ":\n" + source + "\n" + e.what() );
                }
            }
        } );
    }
}


namespace ttb::resources
{
    namespace
    {
        std::string loadSource( std::string const& rootPath, std::string const& filename )
        {
            auto const source = loadFile( filename );

            std::ostringstream outStream;

            std::istringstream inStream{ source };

            for( std::string line; std::getline( inStream, line ); )
            {
                std::smatch match;
                if( std::regex_match( line, match, k_preProcCmd ) )
                {
                    auto const& cmd = match[ 1 ].str();

                    if( cmd == "version" )
                    {
#if defined( PLATFORM_LINUX )
                        // line = "#version 330";
                        line = "#version 100";
#elif defined( PLATFORM_ANDROID )
                        // line = "#version 320 es";
                        line = "#version 100";
#elif defined( PLATFORM_BROWSER )
                        line = "#version 300 es";
#endif
                    }
                    else if( cmd == "include" )
                    {
                        // auto const subFile = dirname( filename ) + '/' + match[ 2 ].str();
                        auto const subFile = rootPath + '/' + match[ 2 ].str();
                        line = loadSource( rootPath, subFile );
                    }
                }

                outStream << line << '\n';
            }

            return outStream.str();
        }
    }
}
