#pragma once

#include <ttb/math/Matrix.hpp>

#include <GL/glew.h>
#include <GL/gl.h>


namespace ttb
{
    namespace helper
    {
        template < typename TType >
        struct set_uniform;

        template <>
        struct set_uniform< float >
        {
            static void apply( GLint location, float value );
        };

        template <>
        struct set_uniform< int >
        {
            static void apply( GLint location, int value );
        };

        template <>
        struct set_uniform< Matrix< float, 3, 3 > >
        {
            static void apply( GLint location, Matrix< float, 3, 3 > const& value );
        };

        template <>
        struct set_uniform< Matrix< float, 4, 4 > >
        {
            static void apply( GLint location, Matrix< float, 4, 4 > const& value );
        };
    }
}
