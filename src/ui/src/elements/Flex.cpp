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
        auto result = Size{};

        result( codirDim() ) = size( codirDim() );

        auto& sizeSum = result( dirDim() );

        for( auto& slot : m_slots )
        {
            switch( slot.type )
            {
                case SlotType::FLEX:
                    result( dirDim() ) = size( dirDim() );
                    goto fit_done;

                case SlotType::FIXED:
                    sizeSum += slot.value;
                    break;

                case SlotType::FIT_INFINITY:
                    if( slot.child )
                    {
                        auto childSize = size;
                        childSize( dirDim() ) = std::numeric_limits< float >::infinity();
                        childSize = slot.child->fit( childSize );
                        sizeSum += childSize( dirDim() );
                    }
                    break;

                case SlotType::FIT:
                    if( slot.child )
                    {
                        auto childSize = size;
                        childSize( dirDim() ) = 0;
                        childSize = slot.child->fit( childSize );
                        sizeSum += childSize( dirDim() );
                    }
                    break;
            }
        }

    fit_done:
        return result;
    }

    void Flex::offset( Offset const& value )
    {
        Element::offset( value );

        auto childOffset = value;

        for( auto const& slot : m_slots )
        {
            if( slot.child )
            {
                slot.child->offset( childOffset );
            }

            childOffset( dirDim() ) += slot.width;
        }
    }

    void Flex::size( Size const& value )
    {
        Element::size( value );

        auto flexSum = 0.0f;
        auto fixedSize = 0.0f;

        for( auto& slot : m_slots )
        {
            switch( slot.type )
            {
                case SlotType::FLEX:
                    flexSum += slot.value;
                    break;

                case SlotType::FIXED:
                    slot.width = slot.value;
                    fixedSize += slot.width;
                    break;

                case SlotType::FIT_INFINITY:
                    if( slot.child )
                    {
                        auto childSize = value;
                        childSize( dirDim() ) = std::numeric_limits< float >::infinity();
                        childSize = slot.child->fit( childSize );
                        slot.width = childSize( dirDim() );
                        fixedSize += slot.width;
                    }
                    break;

                case SlotType::FIT:
                    if( slot.child )
                    {
                        auto childSize = value;
                        childSize( dirDim() ) = 0;
                        childSize = slot.child->fit( childSize );
                        slot.width = childSize( dirDim() );
                        fixedSize += slot.width;
                    }
                    break;
            }
        }

        auto const flexSize = value( dirDim() ) - fixedSize;

        for( auto& slot : m_slots )
        {
            switch( slot.type )
            {
                case SlotType::FLEX:
                    slot.width = flexSize * slot.value / flexSum;
                    break;

                default:
                    break;
            }

            if( slot.child )
            {
                auto childSize = value;
                childSize( dirDim() ) = slot.width;
                slot.child->size( childSize );
            }
        }
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
        for( auto const& slot : m_slots )
        {
            if( slot.child )
            {
                slot.child->render( state );
            }
        }
    }

    bool Flex::onEvent( Event const& event )
    {
        return std::any_of( std::begin( m_slots ),
                            std::end( m_slots ),
                            [ &event ]( auto const& slot )
                            {
                                return slot.child && slot.child->onEvent( event );
                            } );
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

    size_t Flex::addSlot( SlotType type, float value, Element* element, bool isLastChange )
    {
        element->parent( this );

        m_slots.push_back( Slot{ type, value, 0.0f, element } );

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

    Element* Flex::getChild( std::size_t slot )
    {
        return m_slots.at( slot ).child;
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

    size_t Flex::codirDim() const
    {
        return m_direction == Direction::HORIZONTAL ? 1 : 0;
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
