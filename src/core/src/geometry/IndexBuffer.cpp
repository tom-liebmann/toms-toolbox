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
                      GL_DYNAMIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferObject );
    }

    size_t IndexBuffer::size() const
    {
        return m_data.size();
    }

    auto IndexBuffer::modify() -> Modifier
    {
        return { *this };
    }

    std::shared_ptr< IndexBuffer > IndexBuffer::clone() const
    {
        return std::shared_ptr< IndexBuffer >( new IndexBuffer( *this ) );
    }


    void IndexBuffer::Modifier::reserve( size_t size )
    {
        m_buffer->m_data.reserve( size );
    }

    size_t IndexBuffer::Modifier::size() const
    {
        return m_buffer->size();
    }

    uint32_t& IndexBuffer::Modifier::operator[]( size_t index )
    {
        changed( index, index + 1 );
        return *reinterpret_cast< uint32_t* >( m_buffer->m_data.data() + index );
    }

    void IndexBuffer::Modifier::resize( size_t size )
    {
        m_clear = true;
        m_buffer->m_data.resize( size, 0 );
    }

    void IndexBuffer::Modifier::push_back( size_t value )
    {
        m_clear = true;
        m_buffer->m_data.push_back( value );
    }

    void IndexBuffer::Modifier::flush()
    {
        if( !m_clear && m_begin == m_end )
        {
            // No changes
            return;
        }

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_buffer->m_bufferObject );

        if( m_clear )
        {
            glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                          m_buffer->m_data.size() * sizeof( GLuint ),
                          reinterpret_cast< GLvoid const* >( m_buffer->m_data.data() ),
                          GL_DYNAMIC_DRAW );
        }
        else
        {
            std::cout << "IndexBuffer: Uploading " << m_begin << " - " << m_end << '\n';
            glBufferSubData(
                GL_ELEMENT_ARRAY_BUFFER,
                m_begin * sizeof( GLuint ),
                ( m_end - m_begin ) * sizeof( GLuint ),
                reinterpret_cast< GLvoid const* >( m_buffer->m_data.data() + m_begin ) );
        }

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        m_clear = false;
        m_begin = 0;
        m_end = 0;
    }

    IndexBuffer::Modifier::Modifier( IndexBuffer& buffer ) : m_buffer{ &buffer }
    {
    }

    void IndexBuffer::Modifier::changed( size_t begin, size_t end )
    {
        if( m_begin == m_end )
        {
            m_begin = begin;
            m_end = end;
        }
        else
        {
            m_begin = std::min( m_begin, begin );
            m_end = std::max( m_end, end );
        }
    }
}
