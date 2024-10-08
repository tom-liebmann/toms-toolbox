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

        void set_uniform< double >::apply( GLint location, double value )
        {
            glUniform1f( location, static_cast< float >( value ) );
        }

        void set_uniform< int >::apply( GLint location, int value )
        {
            glUniform1i( location, value );
        }

        void set_uniform< std::uint8_t >::apply( GLint location, std::uint8_t value )
        {
            glUniform1i( location, value );
        }

        void set_uniform< Vector< int, 4 > >::apply( GLint location, Vector< int, 4 > const& value )
        {
            glUniform4i( location, value( 0 ), value( 1 ), value( 2 ), value( 3 ) );
        }

        void set_uniform< Vector< float, 4 > >::apply( GLint location,
                                                       Vector< float, 4 > const& value )
        {
            glUniform4f( location, value( 0 ), value( 1 ), value( 2 ), value( 3 ) );
        }

        void set_uniform< Vector< int, 3 > >::apply( GLint location, Vector< int, 3 > const& value )
        {
            glUniform3i( location, value( 0 ), value( 1 ), value( 2 ) );
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

        void set_uniform< Vector< int, 2 > >::apply( GLint location, Vector< int, 2 > const& value )
        {
            glUniform2i( location, value( 0 ), value( 1 ) );
        }

        void set_uniform< Matrix< float, 3, 3 > >::apply( GLint location,
                                                          Matrix< float, 3, 3 > const& value )
        {
            auto const mat = transpose( value );
            glUniformMatrix3fv( location, 1, GL_FALSE, mat.data() );
        }

        void set_uniform< Matrix< double, 3, 3 > >::apply( GLint location,
                                                           Matrix< double, 3, 3 > const& value )
        {
            set_uniform< Matrix< float, 3, 3 > >::apply( location, value.as< float >() );
        }

        void set_uniform< Matrix< float, 4, 4 > >::apply( GLint location,
                                                          Matrix< float, 4, 4 > const& value )
        {
            auto mat = transpose( value );
            glUniformMatrix4fv( location, 1, GL_FALSE, mat.data() );
        }

        void set_uniform< Matrix< double, 4, 4 > >::apply( GLint location,
                                                           Matrix< double, 4, 4 > const& value )
        {
            set_uniform< Matrix< float, 4, 4 > >::apply( location, value.as< float >() );
        }
    }
}
