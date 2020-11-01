#include <ttb/core/geometry/Geometry.hpp>

#include <ttb/core/State.hpp>
#include <ttb/core/geometry/IndexBuffer.hpp>
#include <ttb/core/geometry/VertexBuffer.hpp>
#include <ttb/core/shader/Program.hpp>

#include <limits>

namespace ttb
{
    Geometry::Creator Geometry::create( GLenum mode )
    {
        return { mode };
    }

    Geometry::~Geometry()
    {
        glDeleteVertexArrays( 1, &m_arrayObject );
    }

    Geometry::Geometry( GLenum mode,
                        std::vector< Attribute > attributes,
                        std::shared_ptr< IndexBuffer > const& indexBuffer )
        : m_mode( mode ), m_attributes( std::move( attributes ) ), m_indexBuffer( indexBuffer )
    {
        glGenVertexArrays( 1, &m_arrayObject );
    }

    void Geometry::draw( State& state ) const
    {
        // Apply uniforms to currently bound program
        state.apply();

        // Bind all vertex attributes
        auto const& program = state.program();

        state.pushArrayObject( m_arrayObject );

        std::vector< GLuint > locations;

        for( auto const& attribute : m_attributes )
        {
            auto location = program.attributeLocation( attribute.name() );
            if( location != -1 )
            {
                glEnableVertexAttribArray( location );
                locations.push_back( location );

                attribute.buffer()->bind( attribute.index(), location );
            }
        }

        if( m_indexBuffer )
        {
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer->m_bufferObject );
            glDrawElements(
                m_mode, static_cast< GLsizei >( m_indexBuffer->size() ), GL_UNSIGNED_INT, 0 );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
        }
        else
        {
            size_t minAttrSize = 0;
            if( !m_attributes.empty() )
            {
                minAttrSize = std::numeric_limits< size_t >::max();

                for( auto const& attribute : m_attributes )
                {
                    minAttrSize = std::min( minAttrSize, attribute.buffer()->size() );
                }
            }

            glDrawArrays( m_mode, 0, static_cast< GLsizei >( minAttrSize ) );
        }

        state.popArrayObject();
    }

    Geometry::Attribute const& Geometry::attribute( size_t index ) const
    {
        return m_attributes[ index ];
    }

    Geometry::Attribute& Geometry::attribute( size_t index )
    {
        return m_attributes[ index ];
    }

    GLuint Geometry::arrayObject() const
    {
        return m_arrayObject;
    }



    Geometry::Creator::Creator( GLenum mode ) : m_mode( mode )
    {
    }

    Geometry::Creator& Geometry::Creator::attribute( std::string const& name,
                                                     std::shared_ptr< VertexBuffer > const& buffer,
                                                     size_t index )
    {
        m_attributes.emplace_back( name, buffer, index );
        return *this;
    }

    Geometry::Creator&
        Geometry::Creator::indices( std::shared_ptr< IndexBuffer > const& indexBuffer )
    {
        m_indexBuffer = indexBuffer;
        return *this;
    }

    std::unique_ptr< Geometry > Geometry::Creator::finish()
    {
        return std::unique_ptr< Geometry >(
            new Geometry( m_mode, std::move( m_attributes ), m_indexBuffer ) );
    }



    Geometry::Attribute::Attribute( std::string const& name,
                                    std::shared_ptr< VertexBuffer > const& vertexBuffer,
                                    size_t index )
        : m_name( name ), m_buffer( vertexBuffer ), m_index( index )
    {
    }

    std::string const& Geometry::Attribute::name() const
    {
        return m_name;
    }

    std::shared_ptr< VertexBuffer > const& Geometry::Attribute::buffer() const
    {
        return m_buffer;
    }

    void Geometry::Attribute::buffer( std::shared_ptr< VertexBuffer > buffer, size_t index )
    {
        m_buffer = std::move( buffer );
        m_index = index;
    }

    size_t Geometry::Attribute::index() const
    {
        return m_index;
    }
}
