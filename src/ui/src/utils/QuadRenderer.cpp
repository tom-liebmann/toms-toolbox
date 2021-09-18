#include <ttb/ui/utils/QuadRenderer.hpp>

#include <ttb/core/gl.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/shader.hpp>


namespace ttb::ui
{
    QuadRenderer::QuadRenderer( ttb::resources::Manager& resourceManager )
    {
        m_program = resourceManager.get< ttb::Program >( "ui_quad" );

        m_vertexBuffer = ttb::VertexBuffer::create(
            [ & ]( auto& c )
            {
                c.attribute( GL_FLOAT, 3 );
                c.attribute( GL_FLOAT, 3 );
                c.attribute( GL_FLOAT, 2 );
            } );

        m_indexBuffer = ttb::IndexBuffer::create();

        m_geometry = ttb::Geometry::create( GL_TRIANGLES )
                         .attribute( "in_vertex", m_vertexBuffer, 0 )
                         .attribute( "in_color", m_vertexBuffer, 1 )
                         .attribute( "in_coord", m_vertexBuffer, 2 )
                         .indices( std::move( m_indexBuffer ) )
                         .finish();
    }

    void QuadRenderer::draw( ttb::State& state ) const
    {
        glEnable( GL_BLEND );

        state.with( *m_program,
                    [ & ]
                    {
                        state.draw( *m_geometry );
                    } );

        glDisable( GL_BLEND );
    }

    auto QuadRenderer::add() -> Handle
    {
        size_t const index = m_quads.size();

        static float const coords[] = { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f };

        for( size_t i = 0; i < 16; ++i )
        {
            m_vertexBuffer->push_back()
                .set( 0, 0.0f, 0.0f, 0.0f )
                .set( 1, 0.0f, 0.0f, 0.0f )
                .set( 2, coords[ i * 2 ], coords[ i * 2 + 1 ] );
        }

        // clang-format off
    /*
     *  0  1  2  3
     *  4  5  6  7
     *  8  9 10 11
     * 12 13 14 15
     */
     static size_t const indices[] = {
         // corners
         5, 4, 0, 5, 0, 1,
         6, 2, 3, 6, 3, 7,
         10, 11, 15, 10, 15, 14,
         9, 13, 12, 9, 12, 8,
         // sides
         5, 1, 2, 5, 2, 6,
         6, 7, 11, 6, 11, 10,
         10, 14, 13, 10, 13, 9,
         9, 8, 4, 9, 4, 5,
         // center
         5, 6, 10, 5, 10, 9
     };
        // clang-format on

        for( size_t i = 0; i < sizeof( indices ) / sizeof( size_t ); ++i )
        {
            m_indexBuffer->push_back( index * 16 + indices[ i ] );
        }

        m_quads.push_back( Quad{ { 0.0f, 0.0f }, { 0.0f, 0.0f }, 0.0f } );

        return { *this, index };
    }

    auto QuadRenderer::get( size_t index ) -> Handle
    {
        return { *this, index };
    }

    void QuadRenderer::flush() const
    {
        m_vertexBuffer->flush();
        m_indexBuffer->flush();
    }


    auto QuadRenderer::Handle::color( Color const& value ) const -> Handle const&
    {
        for( size_t i = 0; i < 16; ++i )
        {
            ( *m_renderer.m_vertexBuffer )[ m_index * 16 + i ].set(
                1, value( 0 ), value( 1 ), value( 2 ) );
        }

        return *this;
    }

    auto QuadRenderer::Handle::depth( float value ) const -> Handle const&
    {
        for( size_t i = 0; i < 16; ++i )
        {
            ( *m_renderer.m_vertexBuffer )[ m_index * 16 + i ].setComp( 0, 2, value );
        }

        return *this;
    }

    auto QuadRenderer::Handle::min( float x, float y ) const -> Handle const&
    {
        m_renderer.m_quads[ m_index ].min = ttb::Vector< float, 2 >{ x, y };
        updateVertices();

        return *this;
    }

    auto QuadRenderer::Handle::max( float x, float y ) const -> Handle const&
    {
        m_renderer.m_quads[ m_index ].max = ttb::Vector< float, 2 >{ x, y };
        updateVertices();

        return *this;
    }

    auto QuadRenderer::Handle::radius( float value ) const -> Handle const&
    {
        m_renderer.m_quads[ m_index ].radius = value;
        updateVertices();

        return *this;
    }

    float QuadRenderer::Handle::min( size_t index ) const
    {
        return m_renderer.m_quads[ m_index ].min( index );
    }

    float QuadRenderer::Handle::max( size_t index ) const
    {
        return m_renderer.m_quads[ m_index ].max( index );
    }

    float QuadRenderer::Handle::radius() const
    {
        return m_renderer.m_quads[ m_index ].radius;
    }

    QuadRenderer::Handle::Handle( QuadRenderer& renderer, size_t index )
        : m_renderer{ renderer }, m_index{ index }
    {
    }

    void QuadRenderer::Handle::updateVertices() const
    {
        auto const minX = min( 0 );
        auto const minY = min( 1 );
        auto const maxX = max( 0 );
        auto const maxY = max( 1 );
        auto const radius = this->radius();

        for( size_t i = 0; i < 16; ++i )
        {
            auto attr = ( *m_renderer.m_vertexBuffer )[ m_index * 16 + i ];

            switch( i % 4 )
            {
                case 0:
                    attr.setComp( 0, 0, minX );
                    break;

                case 1:
                    attr.setComp( 0, 0, minX + radius );
                    break;

                case 2:
                    attr.setComp( 0, 0, maxX - radius );
                    break;

                case 3:
                    attr.setComp( 0, 0, maxX );
                    break;
            }

            switch( i / 4 )
            {
                case 0:
                    attr.setComp( 0, 1, minY );
                    break;

                case 1:
                    attr.setComp( 0, 1, minY + radius );
                    break;

                case 2:
                    attr.setComp( 0, 1, maxY - radius );
                    break;

                case 3:
                    attr.setComp( 0, 1, maxY );
                    break;
            }
        }
    }
}
