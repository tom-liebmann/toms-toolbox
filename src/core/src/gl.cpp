#include <ttb/core/gl.hpp>

#include <stdexcept>
#include <string>


void checkGlError()
{
    checkGlError( "" );
}

void checkGlError( std::string const& info )
{
    auto const error = glGetError();

    if( error != GL_NO_ERROR )
    {
        throw std::runtime_error( "OpenGL Error: " + std::to_string( error ) + " " + info );
    }
}
