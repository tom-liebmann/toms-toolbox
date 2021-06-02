#include <ttb/ui/elements/core/Image.hpp>

#include <ttb/core/geometry.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    Image::~Image() = default;

    void Image::image( std::shared_ptr< Texture2D const > image )
    {
        m_image = std::move( image );

        Range const sourceRange = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
        // m_texTransform = ttb::mat::transform( sourceRange, m_image->range() );
        m_texTransform = ttb::mat::transform( sourceRange, sourceRange );
    }

    void Image::range( Range const& range )
    {
        Element::range( range );

        auto const sourceRange = Range{ { 0.0f, 0.0f }, { 1.0f, 1.0f } };
        m_transform = ttb::mat::transform( sourceRange, range );
    }

    void Image::render( ttb::State& state ) const
    {
        auto const u1 =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );
        auto const u2 =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_texTransform" ).push( m_texTransform );
        auto const u3 = state.uniform< int >( "u_texture" ).push( 0 );

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        state.with( *m_program, [ & ] {
            m_image->bind( 0 );
            m_geometry->draw( state );
            m_image->unbind( 0 );
        } );

        glDisable( GL_BLEND );
    }

    Image::Image( Framework& framework, std::shared_ptr< Texture2D const > image )
        : Element{ framework }
    {
        auto vertexBuffer = ttb::VertexBuffer::create( [ & ]( auto& c ) {
            c.attribute( GL_FLOAT, 2 );  //
        } );

        vertexBuffer->push_back().set( 0, 0.0f, 0.0f );
        vertexBuffer->push_back().set( 0, 1.0f, 0.0f );
        vertexBuffer->push_back().set( 0, 0.0f, 1.0f );
        vertexBuffer->push_back().set( 0, 1.0f, 1.0f );
        vertexBuffer->flush();

        m_geometry = ttb::Geometry::create( GL_TRIANGLE_STRIP )
                         .attribute( "in_vertex", std::move( vertexBuffer ), 0 )
                         .finish();

        framework.resourceManager().get< ttb::Program >( "ui_image" );

        this->image( std::move( image ) );
    }
}
