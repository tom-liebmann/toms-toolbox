#include <ttb/core/geometry/IndexBuffer.hpp>

#include <iostream>


namespace ttb
{
    std::shared_ptr< IndexBuffer > IndexBuffer::create()
    {
        return std::shared_ptr< IndexBuffer >( new IndexBuffer() );
    }

    IndexBuffer::IndexBuffer()
    {
        glGenBuffers( 1, &m_bufferObject );
    }

    IndexBuffer::IndexBuffer( IndexBuffer const& copy ) : m_data( copy.m_data )
    {
        glGenBuffers( 1, &m_bufferObject );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_bufferObject );

        glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                      m_data.size() * sizeof( GLuint ),
                      reinterpret_cast< GLvoid const* >( m_data.data() ),
                      GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferObject );
    }

    std::shared_ptr< IndexBuffer > IndexBuffer::clone() const
    {
        return std::shared_ptr< IndexBuffer >( new IndexBuffer( *this ) );
    }


    IndexBuffer::Modifier::Modifier( std::shared_ptr< IndexBuffer > buffer, size_t start )
        : m_buffer( std::move( buffer ) ), m_begin( start ), m_end( start ), m_clear( false )
    {
    }

    IndexBuffer::Modifier& IndexBuffer::Modifier::reserve( size_t indexCount )
    {
        m_buffer->m_data.reserve( m_end + indexCount );
        return *this;
    }

    IndexBuffer::Modifier& IndexBuffer::Modifier::push( size_t index )
    {
        auto& data = m_buffer->m_data;

        if( data.size() < m_end + 1 )
        {
            data.resize( m_end + 1 );
            m_clear = true;
        }

        data[ m_end ] = static_cast< GLuint >( index );
        ++m_end;

        return *this;
    }

    IndexBuffer::Modifier& IndexBuffer::Modifier::trim()
    {
        m_buffer->m_data.erase( std::next( std::begin( m_buffer->m_data ), m_end ),
                                std::end( m_buffer->m_data ) );
        m_clear = true;
        return *this;
    }

    std::shared_ptr< IndexBuffer > IndexBuffer::Modifier::finish()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_buffer->m_bufferObject );

        if( m_clear )
        {
            glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                          m_buffer->m_data.size() * sizeof( GLuint ),
                          reinterpret_cast< GLvoid const* >( m_buffer->m_data.data() ),
                          GL_STATIC_DRAW );
        }
        else
        {
            glBufferSubData(
                GL_ELEMENT_ARRAY_BUFFER,
                m_begin * sizeof( GLuint ),
                ( m_end - m_begin ) * sizeof( GLuint ),
                reinterpret_cast< GLvoid const* >( m_buffer->m_data.data() + m_begin ) );
        }

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        return m_buffer;
    }


    IndexBuffer::Modifier modify( std::shared_ptr< IndexBuffer > buffer, size_t start )
    {
        return { buffer, start };
    }

    IndexBuffer::Modifier modify( std::shared_ptr< IndexBuffer > buffer )
    {
        return { buffer, buffer->m_data.size() };
    }
}
