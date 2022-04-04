#include <ttb/ui/elements/Flex.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/ui/XmlFactory.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Flex >{ "flex" };

        Flex::Direction parseDirection( std::string_view const& value );

        Flex::SlotType parseSlotType( std::string_view const& value );
    }
}


namespace ttb::ui
{
    Flex::Flex( Framework& framework, Direction direction )
        : Element{ framework }, m_direction{ direction }
    {
    }

    Flex::Flex( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : Element{ framework }, m_direction{ Direction::HORIZONTAL }
    {
        if( auto const value = loader.attrValue( node, "direction" ); value )
        {
            m_direction = parseDirection( *value );
        }

        for( auto child = node.first_node(); child; child = child->next_sibling() )
        {
            auto element = loader.loadElement( framework, *child );

            auto slotType = SlotType::FLEX;
            auto slotValue = 1.0f;

            if( auto const value = loader.attrValue( *child, "flex_type" ); value )
            {
                slotType = parseSlotType( *value );
            }

            if( auto const value = loader.attrValue( *child, "flex_value" ); value )
            {
                slotValue = std::stof( std::string{ *value } );
            }

            addSlot( slotType, slotValue, element, false );
        }
    }

    auto Flex::fit( Size const& size ) -> Size
    {
        float fixedSum = 0.0f;
        float flexSum = 0.0f;

        // Compute the overall sums of all flex and fixed elements
        for( auto& slot : m_slots )
        {
            switch( slot.type )
            {
                case SlotType::FLEX:
                    flexSum += slot.value;
                    break;

                case SlotType::FIXED:
                {
                    slot.width = slot.value;

                    if( slot.child )
                    {
                        auto childSize = size;
                        childSize( dirDim() ) = slot.value;
                        slot.child->fit( childSize )( dirDim() );
                    }

                    fixedSum += slot.width;
                    break;
                }

                case SlotType::FIT_INFINITY:
                {
                    slot.width = std::numeric_limits< float >::infinity();

                    if( slot.child )
                    {
                        auto childSize = size;
                        childSize( dirDim() ) = slot.width;
                        slot.width = slot.child->fit( childSize )( dirDim() );
                    }

                    fixedSum += slot.width;
                    break;
                }

                case SlotType::FIT:
                {
                    slot.width = 0.0f;

                    if( slot.child )
                    {
                        auto childSize = size;
                        childSize( dirDim() ) = slot.width;
                        slot.width = slot.child->fit( childSize )( dirDim() );
                    }

                    fixedSum += slot.width;
                    break;
                }
            }
        }

        // Position elements and retrieve maximum height
        auto totalSize = Size{ 0.0f, 0.0f };

        for( auto& slot : m_slots )
        {
            slot.offset = totalSize( dirDim() );

            switch( slot.type )
            {
                case SlotType::FLEX:
                {
                    slot.width = ( size( dirDim() ) - fixedSum ) * slot.value / flexSum;

                    if( slot.child )
                    {
                        auto childSize = size;
                        childSize( dirDim() ) = slot.width;
                        slot.child->fit( childSize );
                    }

                    totalSize( dirDim() ) += slot.width;

                    break;
                }

                case SlotType::FIXED:
                case SlotType::FIT:
                case SlotType::FIT_INFINITY:
                {
                    totalSize( dirDim() ) += slot.width;
                    break;
                }
            }

            if( slot.child )
            {
                totalSize( 1 - dirDim() ) =
                    std::max( totalSize( 1 - dirDim() ), slot.child->size()( 1 - dirDim() ) );
            }
        }

        if( totalSize( 1 - dirDim() ) != size( 1 - dirDim() ) )
        {
            for( auto& slot : m_slots )
            {
                if( slot.child )
                {
                    auto childSize = totalSize;
                    childSize( dirDim() ) = slot.width;
                    slot.child->fit( childSize );
                }
            }
        }

        return Element::fit( totalSize );
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

                state.with( ttb::UniformBinder( "u_transform", ttb::mat::translation( offset ) ),
                            [ & ]
                            {
                                slot.child->render( state );  //
                            } );
            }
        }
    }

    bool Flex::onEvent( Event const& event )
    {
        for( auto const& slot : m_slots )
        {
            if( slot.child )
            {
                auto result = false;

                event.transform(
                    [ &slot, this ]( auto const& v )
                    {
                        auto newV = v;
                        newV( dirDim() ) -= slot.offset;
                        return newV;
                    },
                    [ &slot, &result ]( auto const& event )
                    {
                        result = slot.child->onEvent( event );
                    } );

                if( result )
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
        m_slots.push_back( Slot{ type, value, 0.0f, 0.0f, {} } );

        if( isLastChange )
        {
            changed();
        }

        return m_slots.size() - 1;
    }

    size_t Flex::addSlot( SlotType type, float value, Element* element, bool isLastChange )
    {
        element->parent( this );

        m_slots.push_back( Slot{ type, value, 0.0f, 0.0f, element } );

        if( isLastChange )
        {
            changed();
        }

        return m_slots.size() - 1;
    }

    void Flex::child( size_t slot, Element* element, bool isLastChange )
    {
        element->parent( this );

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

    size_t Flex::dirDim() const
    {
        return m_direction == Direction::HORIZONTAL ? 0 : 1;
    }
}


namespace ttb::ui
{
    namespace
    {
        Flex::Direction parseDirection( std::string_view const& value )
        {
            if( value == "h" )
            {
                return Flex::Direction::HORIZONTAL;
            }

            if( value == "v" )
            {
                return Flex::Direction::VERTICAL;
            }

            throw std::runtime_error( "Unable to parse flex direction" );
        }

        Flex::SlotType parseSlotType( std::string_view const& value )
        {
            if( value == "fixed" )
            {
                return Flex::SlotType::FIXED;
            }

            if( value == "flex" )
            {
                return Flex::SlotType::FLEX;
            }

            if( value == "fit" )
            {
                return Flex::SlotType::FIT;
            }

            if( value == "fit_infinity" )
            {
                return Flex::SlotType::FIT_INFINITY;
            }

            throw std::runtime_error( "Unable to parse slot type" );
        }
    }
}
