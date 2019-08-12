#include <ttb/core/texture/Texture3D.hpp>

namespace ttb
{
    Texture3D::Texture3D() : m_width( 0 ), m_height( 0 ), m_depth( 0 )
    {
    }

    std::unique_ptr< Texture3DModifier > Texture3D::modify()
    {
        return std::unique_ptr< Texture3DModifier >( new Texture3DModifier( *this ) );
    }

    uint16_t Texture3D::width() const
    {
        return m_width;
    }

    uint16_t Texture3D::height() const
    {
        return m_height;
    }

    uint16_t Texture3D::depth() const
    {
        return m_depth;
    }

    void Texture3D::bind( uint8_t slot ) const
    {
        glActiveTexture( GL_TEXTURE0 + slot );
        glBindTexture( GL_TEXTURE_3D, object() );
    }

    void Texture3D::unbind( uint8_t slot ) const
    {
        glActiveTexture( GL_TEXTURE0 + slot );
        glBindTexture( GL_TEXTURE_3D, 0 );
    }


    Texture3DModifier::Texture3DModifier( Texture3D& texture ) : m_texture( texture )
    {
        glBindTexture( GL_TEXTURE_3D, m_texture.object() );
    }

    Texture3DModifier::~Texture3DModifier()
    {
        glBindTexture( GL_TEXTURE_3D, 0 );
    }

    void Texture3DModifier::upload( uint16_t width,
                                    uint16_t height,
                                    uint16_t depth,
                                    GLint internalFormat,
                                    GLenum format,
                                    GLenum valueType,
                                    void const* data ) const
    {
        m_texture.m_width = width;
        m_texture.m_height = height;
        m_texture.m_depth = depth;
        m_texture.m_internalFormat = internalFormat;
        m_texture.m_format = format;
        m_texture.m_valueType = valueType;

        glTexImage3D( GL_TEXTURE_3D,
                      0,
                      m_texture.m_internalFormat,
                      m_texture.m_width,
                      m_texture.m_height,
                      m_texture.m_depth,
                      0,
                      m_texture.m_format,
                      m_texture.m_valueType,
                      data );
    }

    void Texture3DModifier::minMagFilter( GLint minFilter, GLint magFilter ) const
    {
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, minFilter );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, magFilter );
    }

    void Texture3DModifier::wrap( GLenum xWrap, GLenum yWrap, GLenum zWrap ) const
    {
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, xWrap );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, yWrap );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, zWrap );
    }

    void Texture3DModifier::borderColor( float r, float g, float b, float a ) const
    {
        GLfloat col[ 4 ];
        col[ 0 ] = r;
        col[ 1 ] = g;
        col[ 2 ] = b;
        col[ 3 ] = a;
        glTexParameterfv( GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, col );
    }
}
