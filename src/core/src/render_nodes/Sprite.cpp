#include <ttb/render_nodes/Sprite.hpp>


namespace ttb
{
    Sprite::Sprite( Range2F const& range, TextureSlice2D const& texture )
        : m_range{ range }, m_texture{ texture }
    {
        updateTransform();

        m_vertexBuffer = ttb::VertexBuffer::create(
            []( auto& c )
            {
                c.attribute( GL_FLOAT, 2 );
            } );

        m_indexBuffer = ttb::IndexBuffer::create();

        m_geometry = ttb::Geometry::create( GL_TRIANGLES )
                         .attribute( "in_vertex", m_vertexBuffer )
                         .indices( m_indexBuffer )
                         .finish();
    }

    void Sprite::draw( ttb::State& state ) const
    {
        state.with( TextureSlice2DBinder{ m_texture, "", 0 },
                    ttb::UniformBinder{ "u_transform", m_transform },
                    [ & ]
                    {
                        state.draw( *m_geometry );
                    } );
    }

    void Sprite::updateTransform()
    {
        m_transform = ttb::mat::transform( ttb::Range< float, 2 >{ { 0.0f, 0.0f }, { 1.0f, 1.0f } },
                                           m_range );
    }
}
