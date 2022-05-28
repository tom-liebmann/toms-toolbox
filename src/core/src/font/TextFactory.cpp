#include <ttb/core/fonts/TextFactory.hpp>


namespace ttb
{
    std::unique_ptr< ttb::Geometry >
        TextFactory::createText( float size, Font const& font, std::string const& text )
    {
        std::shared_ptr< VertexBuffer > vertexBuffer = VertexBuffer::create(
            [ & ]( auto& c )
            {
                c.attribute( GL_FLOAT, 2 );
                c.attribute( GL_FLOAT, 2 );
            } );

        // push characters
        {
            float scaleFactor = size / font.lineHeight();

            vertexBuffer->reserve( vertexBuffer->numVertices() + text.size() * 6 );

            auto const width = font.width();
            auto const height = font.height();

            float x = 0.0f;
            float y = 0.0f;
            for( size_t i = 0; i < text.size(); ++i )
            {
                char cId = text[ i ];

                if( cId != '\n' )
                {
                    auto const& c = font.character( text[ i ] );

                    // upper left triangle
                    vertexBuffer->push_back()
                        .set( 0, c.x() / width, c.y() / height )
                        .set( 1,
                              ( x + c.xOffset() ) * scaleFactor,
                              ( y + c.yOffset() ) * scaleFactor );

                    vertexBuffer->push_back()
                        .set( 0, c.x() / width, ( c.y() + c.height() ) / height )
                        .set( 1,
                              ( x + c.xOffset() ) * scaleFactor,
                              ( y + c.yOffset() + c.height() ) * scaleFactor );

                    vertexBuffer->push_back()
                        .set( 0, ( c.x() + c.width() ) / width, c.y() / height )
                        .set( 1,
                              ( x + c.xOffset() + c.width() ) * scaleFactor,
                              ( y + c.yOffset() ) * scaleFactor );

                    // lower right triangle
                    vertexBuffer->push_back()
                        .set( 0, c.x() / width, ( c.y() + c.height() ) / height )
                        .set( 1,
                              ( x + c.xOffset() ) * scaleFactor,
                              ( y + c.yOffset() + c.height() ) * scaleFactor );

                    vertexBuffer->push_back()
                        .set( 0, ( c.x() + c.width() ) / width, ( c.y() + c.height() ) / height )
                        .set( 1,
                              ( x + c.xOffset() + c.width() ) * scaleFactor,
                              ( y + c.yOffset() + c.height() ) * scaleFactor );

                    vertexBuffer->push_back()
                        .set( 0, ( c.x() + c.width() ) / width, c.y() / height )
                        .set( 1,
                              ( x + c.xOffset() + c.width() ) * scaleFactor,
                              ( y + c.yOffset() ) * scaleFactor );

                    x += c.xAdvance();
                }
                else
                {
                    x = 0.0f;
                    y += font.lineHeight();
                }
            }

            vertexBuffer->flush();
        }

        return Geometry::create( GL_TRIANGLES )
            .attribute( "in_texCoord", vertexBuffer, 0 )
            .attribute( "in_vertex", vertexBuffer, 1 )
            .finish();
    }
}
