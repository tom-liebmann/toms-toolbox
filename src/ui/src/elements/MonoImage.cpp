#include <ttb/ui/elements/MonoImage.hpp>

#include <ttb/core/geometry.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/core/texture.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/Framework.hpp>


namespace ttb::ui
{
    MonoImage::MonoImage( Framework& framework,
                          std::shared_ptr< Texture2D const > image,
                          ColorRgb const& color )
        : Element{ framework }, m_color( color )
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

        framework.resourceManager().get< ttb::Program >( "ui_mono_image" );

        this->image( std::move( image ) );
    }

    MonoImage::~MonoImage() = default;

    void MonoImage::image( std::shared_ptr< Texture2D const > image )
    {
        m_image = std::move( image );

        Range const sourceRange = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
        // m_texTransform = ttb::mat::transform( sourceRange, m_image->range() );
        m_texTransform = ttb::mat::transform( sourceRange, sourceRange );
    }

    void MonoImage::color( ColorRgb const& value )
    {
        m_color = value;
    }

    void MonoImage::range( Range const& range )
    {
        Element::range( range );

        auto const sourceRange = Range{ { 0.0f, 0.0f }, { 1.0f, 1.0f } };
        m_transform = ttb::mat::transform( sourceRange, range );
    }

    void MonoImage::render( ttb::State& state ) const
    {
        auto const u1 =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );
        auto const u2 =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_texTransform" ).push( m_texTransform );
        auto const u3 = state.uniform< int >( "u_texture" ).push( 0 );
        auto const u4 = state.uniform< ttb::Vector< float, 3 > >( "u_color" )
                            .push( { m_color.rF(), m_color.gF(), m_color.bF() } );

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        state.with( *m_program, [ & ] {
            m_image->bind( 0 );
            m_geometry->draw( state );
            m_image->unbind( 0 );
        } );

        glDisable( GL_BLEND );
    }
}
