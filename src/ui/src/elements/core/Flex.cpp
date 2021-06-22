#include <ttb/ui/elements/core/Flex.hpp>

#include <ttb/math/matrix_operations.hpp>


namespace ttb::ui
{
    auto Flex::modify() -> Modifier
    {
        return { *this };
    }

    void Flex::range( Range const& range )
    {
        Element::range( range );

        m_transform( 0, 2 ) = range.min( 0 );
        m_transform( 1, 2 ) = range.min( 1 );

        float fixedSum = 0.0f;
        float flexSum = 0.0f;

        for( auto const& slot : m_slots )
        {
            switch( slot.type )
            {
                case SlotType::FLEX:
                    flexSum += slot.value;
                    break;

                case SlotType::FIXED:
                    fixedSum += slot.value;
                    break;

                case SlotType::FIT:
                    break;
            }
        }

        for( auto& slot : m_slots )
        {
            if( slot.type == SlotType::FIT )
            {
                if( slot.child )
                {
                    auto childSpace = Range{ { 0.0f, 0.0f }, range.extent() };
                    childSpace.extent( dirDim(), range.extent( dirDim() ) - fixedSum );
                    slot.value = slot.child->fit( childSpace ).extent( dirDim() );
                    fixedSum += slot.value;
                }
            }
        }

        float value = 0.0f;

        for( auto& slot : m_slots )
        {
            auto childRange = Range{ { 0.0f, 0.0f }, range.extent() };
            auto const oldValue = value;

            if( SlotType::FLEX == slot.type )
            {
                value += ( range.extent( dirDim() ) - fixedSum ) * slot.value / flexSum;
            }
            else
            {
                value += slot.value;
            }

            childRange.min( dirDim(), oldValue );
            childRange.max( dirDim(), value );

            if( slot.child )
            {
                slot.child->range( childRange );
            }
        }
    }

    auto Flex::fit( Range const& space ) -> Range
    {
        auto range = space;
        {
            float fixedSum = 0.0f;

            for( auto& slot : m_slots )
            {
                switch( slot.type )
                {
                    case SlotType::FLEX:
                    {
                        // flex elements always fill the entire space
                        return space;
                    }

                    case SlotType::FIXED:
                    {
                        fixedSum += slot.value;
                        break;
                    }

                    case SlotType::FIT:
                        break;
                }
            }

            for( auto& slot : m_slots )
            {
                if( SlotType::FIT == slot.type )
                {
                    if( slot.child )
                    {
                        auto childSpace = Range{ { 0.0f, 0.0f }, space.extent() };
                        childSpace.extent( dirDim(), space.extent( dirDim() ) - fixedSum );
                        slot.value = slot.child->fit( childSpace ).extent( dirDim() );
                        fixedSum += slot.value;
                    }
                }
            }
            range.extent( dirDim(), fixedSum );
        }
        return range;
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
        auto const u =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

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

    Element* Flex::child( std::string const& id )
    {
        size_t const slotId = std::stoi( id );
        return m_slots.at( slotId ).child.get();
    }

    Element const* Flex::child( std::string const& id ) const
    {
        size_t const slotId = std::stoi( id );
        return m_slots.at( slotId ).child.get();
    }

    Flex::Flex( Framework& framework, Direction direction )
        : Element{ framework }
        , m_direction{ direction }
        , m_transform( ttb::mat::identity< float, 3 >() )
    {
    }

    size_t Flex::dirDim() const
    {
        return m_direction == Direction::HORIZONTAL ? 0 : 1;
    }


    Flex::Modifier::~Modifier()
    {
        m_element.get().changed();
    }

    auto Flex::Modifier::slot( SlotType type, float value ) const -> Modifier const&
    {
        m_element.get().m_slots.push_back( Slot{ type, value, {} } );
        return *this;
    }

    auto Flex::Modifier::child( size_t slot, std::unique_ptr< Element > element ) const
        -> Modifier const&
    {
        element->parent( m_element );
        m_element.get().m_slots.at( slot ).child = std::move( element );
        return *this;
    }

    auto Flex::Modifier::slotValue( size_t slot, float value ) const -> Modifier const&
    {
        m_element.get().m_slots.at( slot ).value = value;
        return *this;
    }

    Flex::Modifier::Modifier( Flex& element ) : m_element{ element }
    {
    }
}
