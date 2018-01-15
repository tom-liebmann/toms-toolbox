#include <ttb/core/fonts/TextFactory.hpp>


namespace ttb
{
    std::unique_ptr< ttb::Geometry > TextFactory::createText( Font const& font,
                                                              std::string const& text )
    {
        std::shared_ptr< VertexBuffer > vertexBuffer =
            VertexBuffer::create().attribute( GL_FLOAT, 2 ).attribute( GL_FLOAT, 2 ).finish();

        {
            auto v = vertexBuffer->access();

            float x = 0.0f;
            float y = 0.0f;
            for( size_t i = 0; i < text.size(); ++i )
            {
                char cId = text[ i ];

                if( cId != '\n' )
                {
                    auto const& c = font.character( text[ i ] );
                    // upper left triangle
                    v->push( c.x() / font.texture()->width(), c.y() / font.texture()->height() );
                    v->push( x + c.xOffset(), y + c.yOffset() );

                    v->push( c.x() / font.texture()->width(),
                             ( c.y() + c.height() ) / font.texture()->height() );
                    v->push( x + c.xOffset(), y + c.yOffset() + c.height() );

                    v->push( ( c.x() + c.width() ) / font.texture()->width(),
                             c.y() / font.texture()->height() );
                    v->push( x + c.xOffset() + c.width(), y + c.yOffset() );

                    // lower right triangle
                    v->push( c.x() / font.texture()->width(),
                             ( c.y() + c.height() ) / font.texture()->height() );
                    v->push( x + c.xOffset(), y + c.yOffset() + c.height() );

                    v->push( ( c.x() + c.width() ) / font.texture()->width(),
                             ( c.y() + c.height() ) / font.texture()->height() );
                    v->push( x + c.xOffset() + c.width(), y + c.yOffset() + c.height() );

                    v->push( ( c.x() + c.width() ) / font.texture()->width(),
                             c.y() / font.texture()->height() );
                    v->push( x + c.xOffset() + c.width(), y + c.yOffset() );

                    x += c.xAdvance();
                }
                else
                {
                    x = 0.0f;
                    y += font.lineHeight();
                }
            }
        }

        return Geometry::create( GL_TRIANGLES )
            .attribute( "in_texCoord", vertexBuffer, 0 )
            .attribute( "in_vertex", vertexBuffer, 1 )
            .finish();
    }
}
