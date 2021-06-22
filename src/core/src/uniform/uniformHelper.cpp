#include <ttb/core/uniform/uniformHelper.hpp>

#include <ttb/math/matrix_operations.hpp>


namespace ttb
{
    namespace helper
    {
        void set_uniform< float >::apply( GLint location, float value )
        {
            glUniform1f( location, value );
        }

        void set_uniform< int >::apply( GLint location, int value )
        {
            glUniform1i( location, value );
        }

        void set_uniform< Vector< float, 4 > >::apply( GLint location,
                                                       Vector< float, 4 > const& value )
        {
            glUniform4f( location, value( 0 ), value( 1 ), value( 2 ), value( 3 ) );
        }

        void set_uniform< Vector< float, 3 > >::apply( GLint location,
                                                       Vector< float, 3 > const& value )
        {
            glUniform3f( location, value( 0 ), value( 1 ), value( 2 ) );
        }

        void set_uniform< Vector< float, 2 > >::apply( GLint location,
                                                       Vector< float, 2 > const& value )
        {
            glUniform2f( location, value( 0 ), value( 1 ) );
        }

        void set_uniform< Matrix< float, 3, 3 > >::apply( GLint location,
                                                          Matrix< float, 3, 3 > const& value )
        {
            glUniformMatrix3fv( location, 1, GL_TRUE, value.data() );
        }

        void set_uniform< Matrix< float, 4, 4 > >::apply( GLint location,
                                                          Matrix< float, 4, 4 > const& value )
        {
            auto mat = transpose( value );
            glUniformMatrix4fv( location, 1, GL_FALSE, mat.data() );
        }
    }
}
