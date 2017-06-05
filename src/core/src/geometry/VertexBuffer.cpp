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
        : m_access( nullptr ), m_attributes( std::move( attributes ) )
    {
        glGenBuffers( 1, &m_bufferObject );

        m_blockSize = std::accumulate(
            std::begin( m_attributes ), std::end( m_attributes ), 0,
            []( size_t size, Attribute const& attr ) { return size + attr.byteSize(); } );
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferObject );
    }

    std::shared_ptr< VertexBuffer::Access > VertexBuffer::access()
    {
        if( m_access )
            throw std::runtime_error( "Invalid buffer state" );

        return std::shared_ptr< Access >( new Access( *this ), Access::finish );
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
                glVertexAttribPointer( location, attribute.size(), attribute.type(), GL_FALSE,
                                       m_blockSize, reinterpret_cast< const GLvoid* >( offset ) );
                break;

            case GL_BYTE:
            case GL_UNSIGNED_INT:
                glVertexAttribIPointer( location, attribute.size(), attribute.type(), m_blockSize,
                                        reinterpret_cast< const GLvoid* >( offset ) );
                break;

            default:
                throw std::runtime_error( "Unhandled attribute type" );
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }



    VertexBuffer::Access::Access( VertexBuffer& buffer ) : m_buffer( buffer )
    {
    }

    void VertexBuffer::Access::finish( Access* access )
    {
        glBindBuffer( GL_ARRAY_BUFFER, access->m_buffer.m_bufferObject );
        glBufferData( GL_ARRAY_BUFFER, access->m_buffer.m_data.size(),
                      reinterpret_cast< const GLvoid* >( access->m_buffer.m_data.data() ),
                      GL_STATIC_DRAW );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        access->m_buffer.m_access = nullptr;
        delete access;
    }

    void VertexBuffer::Access::clear()
    {
        m_buffer.m_data.clear();
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

    std::unique_ptr< VertexBuffer > VertexBuffer::Creator::finish()
    {
        return std::unique_ptr< VertexBuffer >( new VertexBuffer( std::move( m_attributes ) ) );
    }
}
