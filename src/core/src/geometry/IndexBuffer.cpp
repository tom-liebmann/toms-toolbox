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

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferObject );
    }

    size_t IndexBuffer::numIndices() const
    {
        return m_data.size();
    }

    void IndexBuffer::reserve( size_t numIndices )
    {
        m_data.reserve( numIndices );
    }

    void IndexBuffer::resize( size_t numIndices )
    {
        m_data.resize( numIndices, 0 );
        m_clear = true;
    }

    void IndexBuffer::clear()
    {
        m_data.clear();
        m_begin = 0;
        m_end = 0;
    }

    auto IndexBuffer::operator[]( size_t index ) -> Index&
    {
        changed( index, index + 1 );
        return m_data[ index ];
    }

    void IndexBuffer::push_back( Index value )
    {
        m_data.push_back( value );
        m_clear = true;
    }

    void IndexBuffer::pop_back()
    {
        m_data.pop_back();
    }

    void IndexBuffer::flush()
    {
        if( !m_clear && m_begin == m_end )
        {
            // No changes
            return;
        }

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_bufferObject );

        if( m_clear )
        {
            glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                          m_data.size() * sizeof( Index ),
                          reinterpret_cast< GLvoid const* >( m_data.data() ),
                          GL_DYNAMIC_DRAW );
        }
        else
        {
            glBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
                             m_begin * sizeof( Index ),
                             ( m_end - m_begin ) * sizeof( Index ),
                             reinterpret_cast< GLvoid const* >( m_data.data() + m_begin ) );
        }

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        m_clear = false;
        m_begin = 0;
        m_end = 0;
    }

    void IndexBuffer::changed( size_t begin, size_t end )
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
