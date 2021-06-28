#include <ttb/ui/elements/Flex.hpp>

#include <ttb/math/matrix_operations.hpp>


namespace ttb::ui
{
    Flex::Flex( Framework& framework, Direction direction )
        : Element{ framework }
        , m_direction{ direction }
        , m_transform( ttb::mat::identity< float, 3 >() )
    {
    }

    auto Flex::fit( Size const& size ) -> Size
    {
        float fixedSum = 0.0f;
        float flexSum = 0.0f;

        auto totalSize = Size{ 0.0f, 0.0f };

        // Compute the overall sums of all flex and fixed elements
        for( auto const& slot : m_slots )
        {
            switch( slot.type )
            {
                case SlotType::FLEX:
                    flexSum += slot.value;
                    break;

                case SlotType::FIXED:
                {
                    fixedSum += slot.value;
                    break;
                }

                case SlotType::FIT:
                {
                    if( slot.child )
                    {
                        auto childSize = size;
                        childSize( dimDir() ) = 0.0f;

                        childSize = slot.child->fit( childSize );

                        slot.value = childSize( dimDir() );
                        totalSize( 1 - dimDir() ) =
                            std::max( totalSize( 1 - dimDir() ), slot.value );
                    }
                    break;
                }
            }
        }

        // Position elements
        for( auto& slot : m_slots )
        {
            switch( slot.type )
            {
                case SlotType::FLEX:
                {
                    break;
                }

                case SlotType::FIXED:
                {
                    auto const childSize = slot.child->fit(
                        { m_direction == Direction::HORIZONTAL ? slot.value : size( 0 ),
                          m_direction == Direction::HORIZONTAL ? size( 1 ) : slot.value } );

                    break;
                }

                case SlotType::FIT:
                {
                    break;
                }
            }

            if( slot.type == SlotType::FIT )
            {
                auto const childSize = Size{ m_direction == Direction::HORIZONTAL ? };
                auto childSize = size;
                childSize( dirDim() ) = 0.0f;

                slot.value = slot.child->fit( childSize )( dirDim() );
                fixedSum += slot.value;
            }
        }

        float offset = 0.0f;

        for( auto& slot : m_slots )
        {
            slot.offset = offset;

            if( SlotType::FLEX == slot.type )
            {
                offset += ( size( dirDim() ) - fixedSum ) * slot.value / flexSum;
            }
            else
            {
                offset += slot.value;
            }
        }

        return Element::fit( { m_direction == Direction::HORIZONTAL ? offset : size( 0 ),
                               m_direction == Direction::HORIZONTAL ? size( 1 ) : offset } );
    }

    void Flex::update( float timeDiff )
    {
        for( auto& slot : m_slots )
        {
            if( slot.child )
            {
                slot.child->update( timeDiff );
            }
        }
    }

    void Flex::render( ttb::State& state ) const
    {
        auto offset = ttb::Vector{ 0.0f, 0.0f };

        for( auto const& slot : m_slots )
        {
            if( slot.child )
            {
                offset( dirDim() ) = slot.offset;

                auto const u = state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" )
                                   .push( ttb::mat::translation( offset ) );

                slot.child->render( state );
            }
        }
    }

    bool Flex::onEvent( Event const& event )
    {
        for( auto const& slot : m_slots )
        {
            if( slot.child )
            {
                if( slot.child->onEvent( event ) )
                {
                    return true;
                }
            }
        }

        return false;
    }

    void Flex::onChildChanged( Element& /* child */ )
    {
        changed();
    }

    std::string Flex::info() const
    {
        return "Flex";
    }

    size_t Flex::addSlot( SlotType type, float value, bool isLastChange )
    {
        m_slots.push_back( Slot{ type, value, 0.0f, {} } );

        if( isLastChange )
        {
            changed();
        }

        return m_slots.size() - 1;
    }

    void Flex::child( size_t slot, Element* element, bool isLastChange )
    {
        element->parent(
            this,
            [ this, slot ]( auto const& pos ) { return transform( slot, pos ); },
            [ this, slot ]( auto const& pos ) { return transformInv( slot, pos ); } );

        m_slots.at( slot ).child = element;

        if( isLastChange )
        {
            changed();
        }
    }

    void Flex::slotValue( size_t slot, float value, bool isLastChange )
    {
        m_slots.at( slot ).value = value;

        if( isLastChange )
        {
            changed();
        }
    }

    auto Flex::transform( size_t slot, Position const& pos ) const -> Position
    {
        auto result = pos;
        result( dirDim() ) += m_slots[ slot ].offset;
        return result;
    }

    auto Flex::transformInv( size_t slot, Position const& pos ) const -> Position
    {
        auto result = pos;
        result( dirDim() ) -= m_slots[ slot ].offset;
        return result;
    }

    size_t Flex::dirDim() const
    {
        return m_direction == Direction::HORIZONTAL ? 0 : 1;
    }
}
