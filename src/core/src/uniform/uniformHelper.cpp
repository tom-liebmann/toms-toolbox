#include <ttb/core/uniform/uniformHelper.hpp>


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

        void set_uniform< Matrix< float, 3, 3 > >::apply( GLint location,
                                                          Matrix< float, 3, 3 > const& value )
        {
            glUniformMatrix3fv( location, 1, GL_TRUE, value.ptr() );
        }

        void set_uniform< Matrix< float, 4, 4 > >::apply( GLint location,
                                                          Matrix< float, 4, 4 > const& value )
        {
            auto mat = transpose( value );
            glUniformMatrix4fv( location, 1, GL_FALSE, mat.ptr() );
        }
    }
}
