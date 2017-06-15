#include <ttb/core/Texture3D.hpp>

namespace ttb
{
    Texture3D::Texture3D() : m_width( 0 ), m_height( 0 ), m_depth( 0 )
    {
        glGenTextures( 1, &m_object );
    }

    Texture3D::~Texture3D()
    {
        glDeleteTextures( 1, &m_object );
    }

    void Texture3D::data( size_t width,
                          size_t height,
                          size_t depth,
                          GLint internalFormat,
                          GLenum format,
                          GLenum type,
                          void* data )
    {
        glBindTexture( GL_TEXTURE_3D, m_object );

        if( m_width == width && m_height == height && m_depth == depth )
        {
            glTexSubImage3D(
                GL_TEXTURE_3D, 0, 0, 0, 0, m_width, m_height, m_depth, format, type, data );
        }
        else
        {
            m_width = width;
            m_height = height;
            m_depth = depth;

            glTexImage3D( GL_TEXTURE_3D,
                          0,
                          internalFormat,
                          m_width,
                          m_height,
                          m_depth,
                          0,
                          format,
                          type,
                          data );
        }

        glBindTexture( GL_TEXTURE_3D, 0 );
    }

    void Texture3D::minMagFilter( GLint minFilter, GLint magFilter )
    {
        glBindTexture( GL_TEXTURE_3D, m_object );

        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, minFilter );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, magFilter );

        glBindTexture( GL_TEXTURE_3D, 0 );
    }

    void Texture3D::wrap( GLenum xWrap, GLenum yWrap, GLenum zWrap )
    {
        glBindTexture( GL_TEXTURE_3D, m_object );

        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, xWrap );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, yWrap );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, zWrap );

        glBindTexture( GL_TEXTURE_3D, 0 );
    }

    void Texture3D::borderColor( float r, float g, float b, float a )
    {
        GLfloat col[ 4 ];
        col[ 0 ] = r;
        col[ 1 ] = g;
        col[ 2 ] = b;
        col[ 3 ] = a;
        glBindTexture( GL_TEXTURE_3D, m_object );
        glTexParameterfv( GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, col );
    }

    void Texture3D::bind( GLenum unit ) const
    {
        glActiveTexture( GL_TEXTURE0 + unit );
        glBindTexture( GL_TEXTURE_3D, m_object );
    }

    size_t Texture3D::width() const
    {
        return m_width;
    }

    size_t Texture3D::height() const
    {
        return m_height;
    }

    size_t Texture3D::depth() const
    {
        return m_depth;
    }
}
