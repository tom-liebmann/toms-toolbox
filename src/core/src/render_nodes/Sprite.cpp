#include <ttb/render_nodes/Sprite.hpp>


namespace
{
    auto const SHADER_SOURCE_VERTEX = R"(
        #version

        precision mediump float;

        uniform mat3 u_transform;
        uniform mat3 u_texTransform;

        attribute vec2 in_vertex;

        varying vec2 frag_coord;

        void main()
        {
            frag_coord = ( u_texTransform * vec3( in_vertex, 1.0 ) ).xy;

            gl_Position = vec4( u_transform * vec3( in_vertex.xy, 1.0 ), 1.0 );
        }
    )";

    auto const SHADER_SOURCE_INDEX = R"(
        #version

        precision mediump float;

        uniform sampler2D u_texture;

        varying vec2 frag_coord;

        void main()
        {
            gl_FragColor = texture2D( u_texture, frag_coord );
        }
    )";
}


namespace ttb
{
    Sprite::Sprite( Range2F const& range, TextureSlice2D const& texture )
        : m_range{ range }, m_texture{ texture }
    {
        updateTransform();

        m_vertexBuffer = VertexBuffer::Builder{}  //
                             .addAttribute( GL_FLOAT, 2 )
                             .build();

        m_indexBuffer = IndexBuffer::Builder{}.build();

        m_geometry = Geometry::Builder{ GL_TRIANGLES }  //
                         .addAttribute( "in_vertex", m_vertexBuffer )
                         .addIndices( m_indexBuffer )
                         .build();

        m_program = Program::Builder{}  //
                        .addShaderSource( ShaderType::VERTEX, SHADER_SOURCE_VERTEX )
                        .addShaderSource( ShaderType::FRAGMENT, SHADER_SOURCE_INDEX )
                        .build();
    }

    void Sprite::draw( ttb::State& state ) const
    {
        state.with( TextureSlice2DBinder{ m_texture, "u_texTransform", 0 },
                    ttb::UniformBinder{ "u_transform", m_transform },
                    *m_program,
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
