#include <ttb/ui/elements/Image.hpp>

#include <ttb/core/geometry.hpp>
#include <ttb/core/resources/Manager.hpp>
#include <ttb/core/shader.hpp>
#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/Framework.hpp>


namespace
{
    auto constexpr TEXTURE_SLOT = std::uint8_t{ 0 };
}


namespace ttb::ui
{
    Image::Image( Framework& framework ) : Element{ framework }
    {
        auto vertexBuffer = ttb::VertexBuffer::create(
            [ & ]( auto& c )
            {
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

        m_program = framework.resourceManager().get< ttb::Program >( "ui_image" );
    }

    Image::Image( Framework& framework, ImageBinder const& image ) : Image{ framework }
    {
        this->image( image );
    }

    Image::~Image() = default;

    void Image::image( ImageBinder const& image )
    {
        m_image = image.getBindable( TEXTURE_SLOT );
    }

    auto Image::fit( Size const& size ) -> Size
    {
        auto const result = Element::fit( size );

        m_transform = ttb::mat::transform( Range{ Vector{ 0.0f, 0.0f }, Vector{ 1.0f, 1.0f } },
                                           Range{ Vector{ 0.0f, 0.0f }, result } );

        return result;
    }

    void Image::render( ttb::State& state ) const
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        state.with( ttb::UniformBinder{ "u_transform", m_transform },
                    ttb::UniformBinder{ "u_texture", TEXTURE_SLOT },
                    *m_image,
                    *m_program,
                    [ & ]
                    {
                        state.draw( *m_geometry );
                    } );

        glDisable( GL_BLEND );
    }
}
