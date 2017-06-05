#include <ttb/core/geometry/IndexBuffer.hpp>

namespace ttb
{
    IndexBuffer::IndexBuffer() : m_access( nullptr )
    {
        glGenBuffers( 1, &m_bufferObject );
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferObject );
    }

    std::shared_ptr< IndexBuffer::Access > IndexBuffer::access()
    {
        if( m_access )
            throw std::runtime_error( "IndexBuffer access failed" );

        return std::shared_ptr< Access >( new Access( *this ), Access::finish );
    }



    IndexBuffer::Access::Access( IndexBuffer& buffer ) : m_buffer( buffer )
    {
        m_buffer.m_access = this;
    }

    void IndexBuffer::Access::finish( Access* access )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, access->m_buffer.m_bufferObject );

        glBufferData( GL_ELEMENT_ARRAY_BUFFER, access->m_buffer.m_data.size() * sizeof( GLuint ),
                      reinterpret_cast< GLvoid const* >( access->m_buffer.m_data.data() ),
                      GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        access->m_buffer.m_access = nullptr;
        delete access;
    }

    void IndexBuffer::Access::push( size_t index )
    {
        m_buffer.m_data.push_back( index );
    }

    void IndexBuffer::Access::clear()
    {
        m_buffer.m_data.clear();
    }
}
