#pragma once

#include <ttb/core/State.hpp>
#include <ttb/core/geometry.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/core/texture.hpp>
#include <ttb/math.hpp>


namespace ttb
{
    class State;
}


namespace ttb
{
    class Sprite
    {
    public:
        Sprite( Range2F const& range, float height, TextureSlice2D const& texture );

        void draw( ttb::State& state ) const;

    private:
        void updateVertices();

        Range2F m_range;
        float m_height;
        TextureSlice2D m_texture;
        std::shared_ptr< ttb::VertexBuffer > m_vertexBuffer;
        std::shared_ptr< ttb::IndexBuffer > m_indexBuffer;
        std::unique_ptr< ttb::Geometry > m_geometry;
        std::unique_ptr< ttb::Program > m_program;
    };
}
