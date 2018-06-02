#include <ttb/core/geometry/VertexBuffer.hpp>

#include <ttb/core/State.hpp>

#include <cassert>
#include <cstring>
#include <numeric>

namespace ttb
{
    VertexBuffer::Creator VertexBuffer::create()
    {
        return {};
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

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferObject );
    }

    void VertexBuffer::bind( size_t index, GLint location ) const
    {
        assert( index < m_attributes.size() );

        glBindBuffer( GL_ARRAY_BUFFER, m_bufferObject );

        GLsizei offset = 0;
        for( GLuint i = 0; i < index; ++i )
            offset += m_attributes[ i ].byteSize();

        auto const& attribute = m_attributes[ index ];

        switch( attribute.type() )
        {
            case GL_FLOAT:
                glVertexAttribPointer( location,
                                       attribute.size(),
                                       attribute.type(),
                                       GL_FALSE,
                                       m_blockSize,
                                       reinterpret_cast< const GLvoid* >( offset ) );
                break;

            case GL_BYTE:
            case GL_UNSIGNED_INT:
                glVertexAttribIPointer( location,
                                        attribute.size(),
                                        attribute.type(),
                                        m_blockSize,
                                        reinterpret_cast< const GLvoid* >( offset ) );
                break;

            default:
                throw std::runtime_error( "Unhandled attribute type" );
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }



    VertexBuffer::Modifier::Modifier( std::shared_ptr< VertexBuffer > buffer, size_t start )
        : m_buffer( std::move( buffer ) ), m_begin( start ), m_end( start ), m_clear( false )
    {
    }

    VertexBuffer::Modifier& VertexBuffer::Modifier::reserve( size_t elementCount )
    {
        m_buffer->m_data.reserve( m_end + elementCount * m_buffer->m_blockSize );
        return *this;
    }

    VertexBuffer::Modifier& VertexBuffer::Modifier::trim()
    {
        m_buffer->m_data.erase( std::next( std::begin( m_buffer->m_data ), m_end ),
                                std::end( m_buffer->m_data ) );
        m_clear = true;
        return *this;
    }

    void VertexBuffer::Modifier::finish()
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_buffer->m_bufferObject );

        if( m_clear )
        {
            glBufferData( GL_ARRAY_BUFFER,
                          m_buffer->m_data.size(),
                          reinterpret_cast< GLvoid const* >( m_buffer->m_data.data() ),
                          GL_STATIC_DRAW );
        }
        else
        {
            glBufferSubData(
                GL_ARRAY_BUFFER,
                m_begin,
                m_end - m_begin,
                reinterpret_cast< GLvoid const* >( m_buffer->m_data.data() + m_begin ) );
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }


    VertexBuffer::Attribute::Attribute( GLenum type, size_t size ) : m_type( type ), m_size( size )
    {
    }

    GLenum VertexBuffer::Attribute::type() const
    {
        return m_type;
    }

    size_t VertexBuffer::Attribute::size() const
    {
        return m_size;
    }

    size_t VertexBuffer::Attribute::byteSize() const
    {
        switch( m_type )
        {
            case GL_FLOAT:
                return sizeof( GLfloat ) * m_size;

            case GL_BYTE:
                return sizeof( GLbyte ) * m_size;

            case GL_UNSIGNED_INT:
                return sizeof( GLuint ) * m_size;

            default:
                throw std::runtime_error( "Unhandled data type " + std::to_string( m_type ) );
        }
    }



    VertexBuffer::Creator::Creator()
    {
    }

    VertexBuffer::Creator& VertexBuffer::Creator::attribute( GLenum type, size_t size )
    {
        m_attributes.emplace_back( type, size );
        return *this;
    }

    std::shared_ptr< VertexBuffer > VertexBuffer::Creator::finish()
    {
        return std::shared_ptr< VertexBuffer >( new VertexBuffer( std::move( m_attributes ) ) );
    }


    VertexBuffer::Modifier modify( std::shared_ptr< VertexBuffer > buffer, size_t start )
    {
        return { buffer, start * buffer->m_blockSize };
    }

    VertexBuffer::Modifier modify( std::shared_ptr< VertexBuffer > buffer )
    {
        return { buffer, buffer->m_data.size() };
    }
}
