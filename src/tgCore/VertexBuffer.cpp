#include <GL/glew.h>

#include "VertexBuffer.hpp"

using tgCore::VertexBuffer;

VertexBuffer::VertexBuffer( std::shared_ptr< VertexAttributeList > attributes )
    : m_editVertices( false )
    , m_vSize( 0 )
    , m_vCapacity( 0 )
    , m_vBuffer( nullptr )
    , m_vBufferObject( -1 )
    , m_editIndices( false )
    , m_iSize( 0 )
    , m_iCapacity( 0 )
    , m_iBuffer( nullptr )
    , m_iBufferObject( -1 )
    , m_attributes( std::move( attributes ) )
{
    glGenBuffers( 1, &m_vBufferObject );
    glGenBuffers( 1, &m_iBufferObject );
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers( 1, &m_vBufferObject );
    glDeleteBuffers( 1, &m_iBufferObject );

    if( m_vBuffer )
        free( m_vBuffer );

    if( m_iBuffer )
        free( m_iBuffer );
}

std::shared_ptr< VertexBuffer::IndexAccess > VertexBuffer::accessIndices()
{
    return m_editIndices
        ? std::shared_ptr< IndexAccess >()
        : std::shared_ptr< IndexAccess >( new IndexAccess( shared_from_this() ), IndexAccess::end );
}

std::shared_ptr< VertexBuffer::VertexAccess > VertexBuffer::accessVertices()
{
    return m_editVertices
        ? std::shared_ptr< VertexAccess >()
        : std::shared_ptr< VertexAccess >( new VertexAccess( shared_from_this() ), VertexAccess::end );
}

void VertexBuffer::render() const
{
    static GLuint usedBuffer = 0;

    if( usedBuffer != m_vBufferObject )
    {
        usedBuffer = m_vBufferObject;

        glBindBufferARB( GL_ARRAY_BUFFER, m_vBufferObject );

        GLuint index = 0;
        size_t offset = 0;

        for( auto& attribute : m_attributes )
        {
            glVertexAttribPointer(
                location,
                attribute->getNum(),
                attribute->getType(),
                GL_FALSE,
                m_attribSize,
                reinterpret_cast< const GLvoid* >( offset ) );
            glEnableVertexAttribArray( location++ );
            offset += attribute->getSize();
        }

        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, m_iBufferObject );
    }

    graphState().applyShaderMatrix();
    glDrawElements( m_mode, m_size, GL_UNSIGNED_INT, 0 );

}

void VertexBuffer::IndexAccess::end( IndexAccess* access )
{
    access->m_vertexBuffer->m_editIndices = false;
    delete access;
}

VertexBuffer::IndexAccess::IndexAccess( std::shared_ptr< VertexBuffer > vertexBuffer )
    : m_vertexBuffer( std::move( vertexBuffer ) )
{
    m_vertexBuffer->m_editIndices = true;
}

void VertexBuffer::IndexAccess::push( void* data, size_t size )
{
    if( m_vertexBuffer->m_iCapacity - m_vertexBuffer->m_iSize < size )
    {
        while( m_vertexBuffer->m_iCapacity - m_vertexBuffer->m_iSize < size )
            m_vertexBuffer->m_iCapacity = std::max< size_t >( sizeof( GLuint ), m_vertexBuffer->m_iCapacity << 1 );

        void* newMem = malloc( m_vertexBuffer->m_iCapacity );

        if( m_vertexBuffer->m_iBuffer )
        {
            memcpy( newMem, m_vertexBuffer->m_iBuffer, m_vertexBuffer->m_iSize );
            free( m_vertexBuffer->m_iBuffer );
        }

        m_vertexBuffer->m_iBuffer = newMem;
    }

    memcpy( reinterpret_cast< uint8_t* >( m_vertexBuffer->m_iBuffer ) + m_vertexBuffer->m_iSize, data, size );
}

void VertexBuffer::VertexAccess::end( VertexAccess* access )
{
    access->m_vertexBuffer->m_editVertices = false;
    delete access;
}

VertexBuffer::VertexAccess::VertexAccess( std::shared_ptr< VertexBuffer > vertexBuffer )
    : m_vertexBuffer( std::move( vertexBuffer ) )
{
    m_vertexBuffer->m_editVertices = true;
}

void VertexBuffer::VertexAccess::push( void* data, size_t size )
{
    if( m_vertexBuffer->m_vCapacity - m_vertexBuffer->m_vSize < size )
    {
        while( m_vertexBuffer->m_vCapacity - m_vertexBuffer->m_vSize < size )
            m_vertexBuffer->m_vCapacity = std::max< size_t >( sizeof( GLfloat ), m_vertexBuffer->m_vCapacity << 1 );

        void* newMem = malloc( m_vertexBuffer->m_vCapacity );

        if( m_vertexBuffer->m_vBuffer )
        {
            memcpy( newMem, m_vertexBuffer->m_vBuffer, m_vertexBuffer->m_vSize );
            free( m_vertexBuffer->m_vBuffer );
        }

        m_vertexBuffer->m_vBuffer = newMem;
    }

    memcpy( reinterpret_cast< uint8_t* >( m_vertexBuffer->m_vBuffer ) + m_vertexBuffer->m_vSize, data, size );
}
