#include "Texture2D.hpp"

using tgCore::Texture2D;

Texture2D::Texture2D(
    uint16_t width,
    uint16_t height,
    GLint internalFormat,
    GLenum format,
    GLenum type )
    : m_width( width )
    , m_height( height )
{
    glGenTextures( 1, &m_textureObject );

    glBindTexture( GL_TEXTURE_2D, m_textureObject );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        m_width,
        m_height,
        0,
        format,
        type,
        NULL );
}

Texture2D::~Texture2D()
{
    glDeleteTextures( 1, &m_textureObject );
}

void Texture2D::setMinMagFilter( GLint minFilter, GLint magFilter )
{
    glBindTexture( GL_TEXTURE_2D, m_textureObject ); 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter ); 
}

void Texture2D::setDepthTextureMode( GLint mode )
{
    glBindTexture( GL_TEXTURE_2D, m_textureObject ); 
    glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, mode );
}

void Texture2D::setTextureCompareMode( GLint mode )
{
    glBindTexture( GL_TEXTURE_2D, m_textureObject ); 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, mode );
}

void Texture2D::setTextureCompareFunc( GLint func )
{
    glBindTexture( GL_TEXTURE_2D, m_textureObject );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, func );
}

void Texture2D::bind( GLenum unit ) const
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_2D, m_textureObject ); 
}
