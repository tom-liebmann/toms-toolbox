#include <ttb/render_nodes/Sprite.hpp>


namespace
{
    auto const SHADER_SOURCE_VERTEX = R"(
        #version 330

        precision mediump float;

        uniform mat4 u_projection;
        uniform mat4 u_view;
        uniform mat4 u_model;
        uniform mat3 u_texTransform;

        attribute vec3 in_vertex;
        attribute vec2 in_texCoord;

        varying vec2 frag_coord;

        void main()
        {
            frag_coord = ( u_texTransform * vec3( in_texCoord, 1.0 ) ).xy;

            gl_Position = u_projection * u_view * u_model * vec4( in_vertex, 1.0 );
        }
    )";

    auto const SHADER_SOURCE_INDEX = R"(
        #version 330

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
    Sprite::Sprite( Range2F const& range, float height, TextureSlice2D const& texture )
        : m_range{ range }, m_height{ height }, m_texture{ texture }
    {
        m_vertexBuffer = VertexBuffer::Builder{}
                             .addAttribute( GL_FLOAT, 3 )  // vertex
                             .addAttribute( GL_FLOAT, 2 )  // texCoord
                             .build();

        m_geometry = Geometry::Builder{ GL_TRIANGLE_FAN }  //
                         .addAttribute( "in_vertex", m_vertexBuffer, 0 )
                         .addAttribute( "in_texCoord", m_vertexBuffer, 1 )
                         .build();

        m_program = Program::Builder{}  //
                        .addShaderSource( ShaderType::VERTEX, SHADER_SOURCE_VERTEX )
                        .addShaderSource( ShaderType::FRAGMENT, SHADER_SOURCE_INDEX )
                        .build();

        updateVertices();
    }

    void Sprite::draw( ttb::State& state ) const
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        state.with( *m_program,
                    TextureSlice2DBinder{ m_texture, "u_texTransform", 0 },
                    [ & ]
                    {
                        state.draw( *m_geometry );
                    } );
    }

    void Sprite::updateVertices()
    {
        m_vertexBuffer->clear();

        m_vertexBuffer->push_back()
            .set( 0, m_range.min( 0 ), m_range.min( 1 ), m_height )
            .set( 1, 0.0f, 0.0f );
        m_vertexBuffer->push_back()
            .set( 0, m_range.max( 0 ), m_range.min( 1 ), m_height )
            .set( 1, 1.0f, 0.0f );
        m_vertexBuffer->push_back()
            .set( 0, m_range.max( 0 ), m_range.max( 1 ), m_height )
            .set( 1, 1.0f, 1.0f );
        m_vertexBuffer->push_back()
            .set( 0, m_range.min( 0 ), m_range.max( 1 ), m_height )
            .set( 1, 0.0f, 1.0f );
        m_vertexBuffer->flush();
    }
}
