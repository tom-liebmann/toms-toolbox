#pragma once

#include <ttb/core/gl.hpp>

#include <stdexcept>


namespace ttb
{
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
    };

    auto convertToGl( ShaderType type ) -> GLenum;
}


namespace ttb
{
    inline auto convertToGl( ShaderType type ) -> GLenum
    {
        switch( type )
        {
            case ShaderType::VERTEX:
                return GL_VERTEX_SHADER;
            case ShaderType::FRAGMENT:
                return GL_FRAGMENT_SHADER;
            case ShaderType::GEOMETRY:
                return GL_GEOMETRY_SHADER;
            default:
                throw std::runtime_error( "Invalid shader type value" );
        }
    }
}
