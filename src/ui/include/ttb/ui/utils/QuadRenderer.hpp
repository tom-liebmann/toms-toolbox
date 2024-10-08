#pragma once

#include <ttb/core/State.hpp>
#include <ttb/core/geometry.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/utils/Color.hpp>


namespace ttb::ui
{
    class QuadRenderer
    {
    public:
        class Handle;

        QuadRenderer( ttb::resources::Manager& resourceManager );

        void draw( ttb::State& state ) const;

        Handle add();

        Handle get( size_t index );

        void flush() const;

    private:
        struct Quad
        {
            ttb::Vector< float, 2 > min;
            ttb::Vector< float, 2 > max;
            float radius;
        };

        std::vector< Quad > m_quads;

        std::shared_ptr< ttb::VertexBuffer > m_vertexBuffer;
        std::shared_ptr< ttb::IndexBuffer > m_indexBuffer;
        std::unique_ptr< ttb::Geometry > m_geometry;
        std::shared_ptr< ttb::Program > m_program;
    };


    class QuadRenderer::Handle
    {
    public:
        Handle const& color( ttb::ColorRgb const& value ) const;

        Handle const& depth( float value ) const;

        Handle const& min( float x, float y ) const;

        Handle const& max( float x, float y ) const;

        Handle const& radius( float value ) const;

        float min( size_t index ) const;

        float max( size_t index ) const;

        float radius() const;

    private:
        Handle( QuadRenderer& renderer, size_t index );

        void updateVertices() const;

        QuadRenderer& m_renderer;
        size_t m_index;

        friend class QuadRenderer;
    };
}
