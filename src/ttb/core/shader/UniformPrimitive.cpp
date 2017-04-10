#include "UniformPrimitive.hpp"

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

        void set_uniform< Matrix< float, 4, 4 > >::apply( GLint location,
                                                          Matrix< float, 4, 4 > const& value )
        {
            glUniformMatrix4fv( location, 1, GL_TRUE, value.ptr() );
        }
    }
}
