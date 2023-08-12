#include <ttb/core/fonts/TextFactory.hpp>

#include <ttb/core/fonts/Glyph.hpp>


namespace ttb
{

    ttb::Range< float, 2 >
        TextFactory::getDimensions( Font const& font, float size, std::string_view text )
    {
        auto dimensions = ttb::Range< float, 2 >{
            { std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity() },
            { -std::numeric_limits< float >::infinity(), -std::numeric_limits< float >::infinity() }
        };

        auto const scaleFactor = size / font.getEmSize();

        auto x = 0.0f;
        auto y = 0.0f;
        for( size_t i = 0; i < text.size(); ++i )
        {
            char cId = text[ i ];

            if( cId == '\n' )
            {
                x = 0.0f;
                y += font.getLineHeight() * scaleFactor;
            }
            else
            {
                auto const& glyph = font.getGlyph( text[ i ] );

                dimensions.expand( {
                    x + glyph.getRange().getMin( 0 ) * scaleFactor,
                    y + ( 1.0f - glyph.getRange().getMin( 1 ) ) * scaleFactor,
                    // y + ( glyph.getRange().getMin( 1 ) ) * scaleFactor,
                } );

                dimensions.expand( {
                    x + glyph.getRange().getMax( 0 ) * scaleFactor,
                    y + ( 1.0f - glyph.getRange().getMax( 1 ) ) * scaleFactor,
                    // y + ( glyph.getRange().getMax( 1 ) ) * scaleFactor,
                } );

                x += glyph.getAdvance() * scaleFactor;
            }
        }

        return dimensions;
    }

    std::unique_ptr< ttb::Geometry >
        TextFactory::createText( Font const& font, float size, std::string_view text )
    {
        std::shared_ptr< ttb::VertexBuffer > vertexBuffer = VertexBuffer::create(
            [ & ]( auto& c )
            {
                c.attribute( GL_FLOAT, 2 );  // in_texCoord
                c.attribute( GL_FLOAT, 2 );  // in_vertex
            } );

        auto indexBuffer = IndexBuffer::create();

        // push characters
        {
            auto const scaleFactor = size / font.getEmSize();

            vertexBuffer->reserve( text.size() * 4 );
            indexBuffer->reserve( text.size() * 6 );

            auto x = 0.0f;
            auto y = 0.0f;
            for( size_t i = 0; i < text.size(); ++i )
            {
                char cId = text[ i ];

                if( cId == '\n' )
                {
                    x = 0.0f;
                    y += font.getLineHeight() * scaleFactor;
                }
                else
                {
                    auto const& glyph = font.getGlyph( cId );

                    // Push vertices
                    vertexBuffer->push_back()
                        .set( 0,
                              glyph.getTexRange().getMin( 0 ) / ( font.getTexWidth() - 1 ),
                              1.0f - glyph.getTexRange().getMin( 1 ) / ( font.getTexHeight() - 1 ) )
                        .set( 1,
                              x + glyph.getRange().getMin( 0 ) * scaleFactor,
                              y + ( 1.0f - glyph.getRange().getMin( 1 ) ) * scaleFactor );

                    vertexBuffer->push_back()
                        .set( 0,
                              glyph.getTexRange().getMax( 0 ) / ( font.getTexWidth() - 1 ),
                              1.0f - glyph.getTexRange().getMin( 1 ) / ( font.getTexHeight() - 1 ) )
                        .set( 1,
                              x + glyph.getRange().getMax( 0 ) * scaleFactor,
                              y + ( 1.0f - glyph.getRange().getMin( 1 ) ) * scaleFactor );

                    vertexBuffer->push_back()
                        .set( 0,
                              glyph.getTexRange().getMax( 0 ) / ( font.getTexWidth() - 1 ),
                              1.0f - glyph.getTexRange().getMax( 1 ) / ( font.getTexHeight() - 1 ) )
                        .set( 1,
                              x + glyph.getRange().getMax( 0 ) * scaleFactor,
                              y + ( 1.0f - glyph.getRange().getMax( 1 ) ) * scaleFactor );

                    vertexBuffer->push_back()
                        .set( 0,
                              glyph.getTexRange().getMin( 0 ) / ( font.getTexWidth() - 1 ),
                              1.0f - glyph.getTexRange().getMax( 1 ) / ( font.getTexHeight() - 1 ) )
                        .set( 1,
                              x + glyph.getRange().getMin( 0 ) * scaleFactor,
                              y + ( 1.0f - glyph.getRange().getMax( 1 ) ) * scaleFactor );

                    // Push triangle indices
                    indexBuffer->push_back( i * 4 + 0 );
                    indexBuffer->push_back( i * 4 + 1 );
                    indexBuffer->push_back( i * 4 + 3 );
                    indexBuffer->push_back( i * 4 + 1 );
                    indexBuffer->push_back( i * 4 + 2 );
                    indexBuffer->push_back( i * 4 + 3 );

                    x += glyph.getAdvance() * scaleFactor;
                }
            }

            vertexBuffer->flush();
            indexBuffer->flush();
        }

        return Geometry::create( GL_TRIANGLES )
            .attribute( "in_texCoord", vertexBuffer, 0 )
            .attribute( "in_vertex", vertexBuffer, 1 )
            .indices( indexBuffer )
            .finish();
    }
}
