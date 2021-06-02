#include <ttb/ui/elements/core/RowLayout.hpp>


namespace ttb::ui
{
    void RowLayout::child( uint16_t index, std::unique_ptr< Element > element )
    {
        if( element )
        {
            element->parent( *this );
        }

        auto const iter =
            std::find_if( std::begin( m_children ),
                          std::end( m_children ),
                          [ & ]( auto const& child ) { return child.index == index; } );

        if( iter == std::end( m_children ) )
        {
            m_children.push_back( Child{ index, std::move( element ) } );
            std::sort( std::begin( m_children ), std::end( m_children ) );
        }
        else
        {
            if( iter->element )
            {
                iter->element->destroy();
            }

            iter->element = std::move( element );
        }

        changed();
    }

    void RowLayout::erase( uint16_t index )
    {
        auto const iter =
            std::find_if( std::begin( m_children ),
                          std::end( m_children ),
                          [ & ]( auto const& child ) { return child.index == index; } );

        if( iter != std::end( m_children ) )
        {
            m_children.erase( iter );
        }

        changed();
    }

    uint16_t RowLayout::findChild( Element const& element )
    {
        auto const iter =
            std::find_if( std::begin( m_children ),
                          std::end( m_children ),
                          [ & ]( auto const& child ) { return child.element.get() == &element; } );

        if( iter == std::end( m_children ) )
        {
            return m_children.size();
        }
        else
        {
            return std::distance( std::begin( m_children ), iter );
        }
    }

    uint16_t RowLayout::size() const
    {
        return m_children.size();
    }

    void RowLayout::range( Range const& range )
    {
        Element::range( range );

        float rowPos = 0.0f;
        float rowHeight = 0.0f;
        uint16_t currentRow = 0;
        uint16_t rowStart = 0;

        float colWidth = range.extent( 0 ) / m_numCols;

        for( uint16_t i = 0; i < m_children.size(); ++i )
        {
            auto const& c = m_children[ i ];
            auto const col = c.index % m_numCols;
            auto const row = c.index / m_numCols;

            if( row != currentRow )
            {
                for( uint16_t j = rowStart; j < i; ++j )
                {
                    auto const& c2 = m_children[ j ];
                    auto const col2 = c2.index % m_numCols;

                    auto const subRange = Range{ { colWidth * col2, rowPos },
                                                 { colWidth * ( col2 + 1 ), rowPos + rowHeight } };
                    c2.element->range( c2.element->fit( subRange ) );
                }

                rowPos += rowHeight;
                rowHeight = 0.0f;
                currentRow = row;
                rowStart = i;
            }

            auto const subRange =
                Range{ { colWidth * col, rowPos },
                       { colWidth * ( col + 1 ), std::numeric_limits< float >::infinity() } };

            auto const childRange = c.element->fit( subRange );

            rowHeight = std::max( rowHeight, childRange.extent( 1 ) );
        }

        for( uint16_t j = rowStart; j < m_children.size(); ++j )
        {
            auto const& c2 = m_children[ j ];
            auto const col2 = c2.index % m_numCols;

            auto const subRange = Range{ { colWidth * col2, rowPos },
                                         { colWidth * ( col2 + 1 ), rowPos + rowHeight } };
            c2.element->range( c2.element->fit( subRange ) );
        }
    }

    RowLayout::Range RowLayout::fit( Range const& space )
    {
        float rowPos = 0.0f;
        float rowHeight = 0.0f;
        uint16_t currentRow = 0;

        float colWidth = space.extent( 0 ) / m_numCols;

        for( uint16_t i = 0; i < m_children.size(); ++i )
        {
            auto const& c = m_children[ i ];
            auto const col = c.index % m_numCols;
            auto const row = c.index / m_numCols;

            if( row != currentRow )
            {
                rowPos += rowHeight;
                rowHeight = 0.0f;
                currentRow = row;
            }

            auto const subRange =
                Range{ { colWidth * col, rowPos },
                       { colWidth * ( col + 1 ), std::numeric_limits< float >::infinity() } };

            auto const childRange = c.element->fit( subRange );

            rowHeight = std::max( rowHeight, childRange.extent( 1 ) );
        }

        auto const range =
            Range{ space.min(), { space.max( 0 ), space.min( 1 ) + rowPos + rowHeight } };
        return range;
    }

    void RowLayout::update( float timeDiff )
    {
        for( auto& c : m_children )
        {
            c.element->update( timeDiff );
        }
    }

    void RowLayout::render( ttb::State& state ) const
    {
        auto const transform = ttb::mat::translation( Element::range().min() );

        auto const u1 =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( transform );

        for( auto const& c : m_children )
        {
            if( c.element )
            {
                c.element->render( state );
            }
        }
    }

    bool RowLayout::onEvent( Event const& event )
    {
        for( auto& c : m_children )
        {
            if( c.element->onEvent( event ) )
            {
                return true;
            }
        }

        return false;
    }

    RowLayout::RowLayout( Framework& framework, uint16_t numCols )
        : Element{ framework }, m_numCols{ numCols }
    {
    }


    bool RowLayout::Child::operator<( Child const& rhs ) const
    {
        return index < rhs.index;
    }
}
