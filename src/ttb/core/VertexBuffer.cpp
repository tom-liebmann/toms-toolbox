#include "VertexBuffer.hpp"

#include <ttb/core/State.hpp>
#include <ttb/core/VertexAttribute.hpp>

#include <cstring>

ttb::VertexBuffer::VertexBuffer( GLenum mode, std::shared_ptr< VertexAttributeList > attributes )
    : m_access( nullptr )
    , m_vBufferObject( -1 )
    , m_iBufferObject( -1 )
    , m_mode( mode )
    , m_attributes( std::move( attributes ) )
{
    glGenBuffers( 1, &m_vBufferObject );
    glGenBuffers( 1, &m_iBufferObject );
}

ttb::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers( 1, &m_vBufferObject );
    glDeleteBuffers( 1, &m_iBufferObject );

    if( m_access )
        m_access->m_vertexBuffer = nullptr;
}

std::shared_ptr< ttb::VertexBuffer::Access > ttb::VertexBuffer::access()
{
    return m_access ? std::shared_ptr< Access >()
                    : std::shared_ptr< Access >( new Access( this ), Access::end );
}

void ttb::VertexBuffer::render( State& state ) const
{
    // do not render while buffers get modified
    if( m_access )
        return;

    state.apply();

    glBindBuffer( GL_ARRAY_BUFFER, m_vBufferObject );

    GLsizei offset = 0;
    for( GLuint index = 0; index < m_attributes->size(); ++index )
    {
        const auto& attribute = ( *m_attributes )[ index ];

        switch( attribute.getType() )
        {
            case GL_FLOAT:
                glVertexAttribPointer( index, attribute.getSize(), attribute.getType(), GL_FALSE,
                                       m_attributes->getBlockSize(),
                                       reinterpret_cast< const GLvoid* >( offset ) );
                break;

            case GL_BYTE:
            case GL_UNSIGNED_INT:
                glVertexAttribIPointer( index, attribute.getSize(), attribute.getType(),
                                        m_attributes->getBlockSize(),
                                        reinterpret_cast< const GLvoid* >( offset ) );
                break;
        }

        glEnableVertexAttribArray( index );

        if( index != m_attributes->size() - 1 )
        {
            switch( attribute.getType() )
            {
                case GL_FLOAT:
                    offset += sizeof( GLfloat ) * attribute.getSize();
                    break;
                case GL_BYTE:
                    offset += sizeof( GLbyte ) * attribute.getSize();
                    break;
                case GL_UNSIGNED_INT:
                    offset += sizeof( GLuint ) * attribute.getSize();
                    break;
            }
        }
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_iBufferObject );
    glDrawElements( m_mode, m_iBuffer.size(), GL_UNSIGNED_INT, 0 );

    for( GLuint index = 0; index < m_attributes->size(); ++index )
    {
        glDisableVertexAttribArray( index );
    }
}



void ttb::VertexBuffer::Access::end( Access* access )
{
    if( access->m_iChanged )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, access->m_vertexBuffer->m_iBufferObject );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                      access->m_vertexBuffer->m_iBuffer.size() * sizeof( GLuint ),
                      reinterpret_cast< const GLvoid* >( access->m_vertexBuffer->m_iBuffer.data() ),
                      GL_STATIC_DRAW );
    }

    if( access->m_vChanged )
    {
        glBindBuffer( GL_ARRAY_BUFFER, access->m_vertexBuffer->m_vBufferObject );
        glBufferData( GL_ARRAY_BUFFER, access->m_vertexBuffer->m_vBuffer.size(),
                      reinterpret_cast< const GLvoid* >( access->m_vertexBuffer->m_vBuffer.data() ),
                      GL_STATIC_DRAW );
    }

    access->m_vertexBuffer->m_access = nullptr;
    delete access;
}

ttb::VertexBuffer::Access::Access( ttb::VertexBuffer* vertexBuffer )
    : m_vertexBuffer( vertexBuffer ), m_iChanged( false ), m_vChanged( false )
{
}
