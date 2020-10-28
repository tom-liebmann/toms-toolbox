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

    size_t VertexBuffer::size() const
    {
        return m_data.size() / m_blockSize;
    }

    std::unique_ptr< VertexBuffer > VertexBuffer::clone() const
    {
        return std::unique_ptr< VertexBuffer >{ new VertexBuffer{ *this } };
    }

    VertexBuffer::VertexBuffer( std::vector< Attribute > attributes )
        : m_attributes( std::move( attributes ) )
    {
        glGenBuffers( 1, &m_bufferObject );

        m_blockSize = std::accumulate(
            std::begin( m_attributes ),
            std::end( m_attributes ),
            0,
            []( size_t size, Attribute const& attr ) { return size + attr.byteSize(); } );
    }

    VertexBuffer::VertexBuffer( VertexBuffer const& copy )
        : m_data( copy.m_data ), m_attributes( copy.m_attributes ), m_blockSize{ copy.m_blockSize }

    {
        glGenBuffers( 1, &m_bufferObject );

        glBindBuffer( GL_ARRAY_BUFFER, m_bufferObject );

        glBufferData( GL_ARRAY_BUFFER,
                      m_data.size(),
                      reinterpret_cast< GLvoid const* >( m_data.data() ),
                      GL_DYNAMIC_DRAW );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
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
                                       static_cast< GLsizei >( m_blockSize ),
                                       reinterpret_cast< const GLvoid* >( offset ) );
                break;

            case GL_BYTE:
                // case GL_UNSIGNED_INT:
                //     glVertexAttribIPointer( location,
                //                             attribute.size(),
                //                             attribute.type(),
                //                             m_blockSize,
                //                             reinterpret_cast< const GLvoid* >( offset ) );
                //     break;

            default:
                throw std::runtime_error( "Unhandled attribute type" );
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
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


    void VertexBuffer::Modifier::reserve( size_t elementCount )
    {
        auto& data = m_buffer.m_data;
        auto const newCapacity = elementCount * m_buffer.m_blockSize;

        if( newCapacity > data.capacity() )
        {
            data.reserve( newCapacity );
            m_clear = true;
        }
    }

    void VertexBuffer::Modifier::resize( size_t elementCount )
    {
        auto& data = m_buffer.m_data;
        auto const newSize = elementCount * m_buffer.m_blockSize;

        if( newSize > data.capacity() )
        {
            m_clear = true;
            data.resize( newSize );
        }
        else if( newSize > data.size() )
        {
            changed( data.size(), newSize );
            data.resize( newSize );
        }
        else
        {
            m_begin = std::min( m_begin, newSize );
            m_end = std::min( m_end, newSize );
            data.resize( newSize );
        }
    }

    size_t VertexBuffer::Modifier::size() const
    {
        return m_buffer.size();
    }

    void VertexBuffer::Modifier::pop_back()
    {
        auto& data = m_buffer.m_data;
        data.resize( data.size() - m_buffer.m_blockSize );
    }

    void VertexBuffer::Modifier::clear()
    {
        m_buffer.m_data.clear();
        m_begin = 0;
        m_end = 0;
    }

    auto VertexBuffer::Modifier::push_back() -> AttributeHandle
    {
        auto const elementCount = m_buffer.size();
        resize( elementCount + 1 );
        return { m_buffer, elementCount };
    }

    auto VertexBuffer::Modifier::operator[]( size_t index ) -> AttributeHandle
    {
        changed( index * m_buffer.m_blockSize, ( index + 1 ) * m_buffer.m_blockSize );
        return { m_buffer, index };
    }

    VertexBuffer::Modifier::Modifier( VertexBuffer& buffer ) : m_buffer{ buffer }
    {
    }

    VertexBuffer::Modifier::~Modifier()
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_buffer.m_bufferObject );

        auto& data = m_buffer.m_data;

        if( m_clear )
        {
            glBufferData( GL_ARRAY_BUFFER,
                          data.capacity(),
                          reinterpret_cast< GLvoid const* >( data.data() ),
                          GL_DYNAMIC_DRAW );
        }
        else
        {
            glBufferSubData( GL_ARRAY_BUFFER,
                             m_begin,
                             m_end - m_begin,
                             reinterpret_cast< GLvoid const* >( data.data() + m_begin ) );
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    void VertexBuffer::Modifier::changed( size_t begin, size_t end )
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
