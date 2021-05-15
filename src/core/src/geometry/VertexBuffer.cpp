#include <ttb/core/geometry/VertexBuffer.hpp>

#include <ttb/core/State.hpp>

#include <cassert>
#include <cstring>
#include <numeric>

namespace ttb
{
    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferObject );
    }

    size_t VertexBuffer::numVertices() const
    {
        return byteSize() / m_vertexSize;
    }

    size_t VertexBuffer::byteSize() const
    {
        return m_data.size();
    }

    void VertexBuffer::reserve( size_t numVertices )
    {
        auto const newCapacity = numVertices * m_vertexSize;

        if( newCapacity > m_data.capacity() )
        {
            m_data.reserve( newCapacity );
            m_clear = true;
        }
    }

    void VertexBuffer::resize( size_t numVertices )
    {
        auto const newSize = numVertices * m_vertexSize;

        if( newSize > m_data.capacity() )
        {
            m_data.resize( newSize );
            m_clear = true;
        }
        else if( newSize > m_data.size() )
        {
            changed( m_data.size(), newSize );
            m_data.resize( newSize );
        }
        else
        {
            m_begin = std::min( m_begin, newSize );
            m_end = std::min( m_end, newSize );
            m_data.resize( newSize );
        }
    }

    void VertexBuffer::pop_back()
    {
        m_data.resize( m_data.size() - m_vertexSize );
    }

    void VertexBuffer::clear()
    {
        m_data.clear();
        m_begin = 0;
        m_end = 0;
    }

    auto VertexBuffer::push_back() -> VertexHandle
    {
        auto const index = numVertices();
        resize( index + 1 );
        return { *this, index };
    }

    auto VertexBuffer::operator[]( size_t index ) -> VertexHandle
    {
        changed( index * m_vertexSize, ( index + 1 ) * m_vertexSize );
        return { *this, index };
    }

    void VertexBuffer::flush()
    {
        if( !m_clear && m_begin == m_end )
        {
            // No changes
            return;
        }

        glBindBuffer( GL_ARRAY_BUFFER, m_bufferObject );

        if( m_clear )
        {
            glBufferData( GL_ARRAY_BUFFER,
                          m_data.capacity(),
                          reinterpret_cast< GLvoid const* >( m_data.data() ),
                          GL_DYNAMIC_DRAW );
        }
        else
        {
            glBufferSubData( GL_ARRAY_BUFFER,
                             m_begin,
                             m_end - m_begin,
                             reinterpret_cast< GLvoid const* >( m_data.data() + m_begin ) );
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        m_clear = false;
        m_begin = 0;
        m_end = 0;
    }



    VertexBuffer::VertexBuffer( std::vector< Attribute > attributes )
        : m_attributes( std::move( attributes ) )
    {
        glGenBuffers( 1, &m_bufferObject );

        m_vertexSize = std::accumulate(
            std::begin( m_attributes ),
            std::end( m_attributes ),
            0,
            []( size_t size, Attribute const& attr ) { return size + attr.byteSize(); } );
    }

    void VertexBuffer::bind( size_t index, GLint location ) const
    {
        assert( index < m_attributes.size() );

        glBindBuffer( GL_ARRAY_BUFFER, m_bufferObject );

        GLsizei offset = 0;
        for( GLuint i = 0; i < index; ++i )
        {
            offset += static_cast< GLsizei >( m_attributes[ i ].byteSize() );
        }

        auto const& attribute = m_attributes[ index ];

        switch( attribute.type )
        {
            case GL_FLOAT:
                glVertexAttribPointer( location,
                                       static_cast< GLint >( attribute.size ),
                                       attribute.type,
                                       GL_FALSE,
                                       static_cast< GLsizei >( m_vertexSize ),
                                       reinterpret_cast< const GLvoid* >( offset ) );
                break;

            case GL_BYTE:
                // case GL_UNSIGNED_INT:
                //     glVertexAttribIPointer( location,
                //                             attribute.size(),
                //                             attribute.type(),
                //                             m_vertexSize,
                //                             reinterpret_cast< const GLvoid* >( offset ) );
                //     break;

            default:
                throw std::runtime_error( "Unhandled attribute type" );
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    void VertexBuffer::changed( size_t begin, size_t end )
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


    size_t VertexBuffer::Attribute::byteSize() const
    {
        switch( type )
        {
            case GL_FLOAT:
                return sizeof( GLfloat ) * size;

            case GL_BYTE:
                return sizeof( GLbyte ) * size;

            case GL_UNSIGNED_INT:
                return sizeof( GLuint ) * size;

            default:
                throw std::runtime_error( "Unhandled data type " + std::to_string( type ) );
        }
    }


    void VertexBuffer::Creator::attribute( GLenum type, size_t size )
    {
        m_attributes.push_back( Attribute{ type, size, 0 } );
    }

    VertexBuffer::Creator::Creator() = default;

    VertexBuffer::Creator::~Creator() = default;

    std::unique_ptr< VertexBuffer > VertexBuffer::Creator::finish()
    {
        size_t offset = 0;
        for( auto& attribute : m_attributes )
        {
            attribute.offset = offset;
            offset += attribute.byteSize();
        }

        return std::unique_ptr< VertexBuffer >{ new VertexBuffer{ std::move( m_attributes ) } };
    }
}
