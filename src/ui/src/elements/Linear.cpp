#include <ttb/ui/elements/Linear.hpp>


namespace ttb::ui
{
    Linear::Linear( Root& root, Direction direction ) : Element{ root }, m_direction{ direction }
    {
        setWidth( Extent::Type::MATCH_CHILD );
        setHeight( Extent::Type::MATCH_CHILD );
    }

    Linear::Linear( Root& root, XmlNode const& node, XmlLoader& loader ) : Element{ root }
    {
    }

    FitExtent Linear::fitWidth( Size const& space ) const
    {
        auto const margin = getMargin();
        auto const childSpace =
            Size{ space( 0 ) - margin.getRightLeft(), space( 1 ) - margin.getTopBottom() };

        if( m_direction == Direction::VERTICAL )
        {
            if( getWidth().getType() != Extent::Type::MATCH_CHILD )
            {
                return Element::fitWidth( space );
            }

            auto maxChildExtent = FitExtent{ 0.0f };

            for( auto const& slot : m_slots )
            {
                auto const childFit = slot.child->fitWidth( childSpace );

                if( childFit.getType() == FitExtent::Type::MATCH_PARENT )
                {
                    return childFit;
                }

                maxChildExtent =
                    FitExtent{ std::max( maxChildExtent.getValue(), childFit.getValue() ) };
            }

            return { maxChildExtent.getValue() + margin.getRightLeft() };
        }

        auto sizeSum = 0.0f;

        for( auto const& slot : m_slots )
        {
            auto const childFit = slot.child->fitWidth( childSpace );

            if( childFit.getType() == FitExtent::Type::MATCH_PARENT )
            {
                return childFit;
            }

            sizeSum += childFit.getValue();
        }

        return { sizeSum + margin.getRightLeft() };
    }

    FitExtent Linear::fitHeight( Size const& space ) const
    {
        auto const margin = getMargin();
        auto const childSpace =
            Size{ space( 0 ) - margin.getRightLeft(), space( 1 ) - margin.getTopBottom() };

        if( m_direction == Direction::HORIZONTAL )
        {
            if( getHeight().getType() != Extent::Type::MATCH_CHILD )
            {
                return Element::fitWidth( space );
            }

            auto maxChildExtent = FitExtent{ 0.0f };

            for( auto const& slot : m_slots )
            {
                auto const childFit = slot.child->fitHeight( childSpace );

                if( childFit.getType() == FitExtent::Type::MATCH_PARENT )
                {
                    return childFit;
                }

                maxChildExtent =
                    FitExtent{ std::max( maxChildExtent.getValue(), childFit.getValue() ) };
            }

            return { maxChildExtent.getValue() + margin.getTopBottom() };
        }

        auto sizeSum = 0.0f;

        for( auto const& slot : m_slots )
        {
            auto const childFit = slot.child->fitHeight( childSpace );

            if( childFit.getType() == FitExtent::Type::MATCH_PARENT )
            {
                return childFit;
            }

            sizeSum += childFit.getValue();
        }

        return { sizeSum + margin.getTopBottom() };
    }

    void Linear::setSize( Size const& value )
    {
        Element::setSize( value );

        auto const& size = getSize();

        auto weightSum = 0.0f;
        auto fixedSize = 0.0f;

        for( auto& slot : m_slots )
        {
            auto const childFit = m_direction == Direction::HORIZONTAL
                                      ? slot.child->fitWidth( size )
                                      : slot.child->fitHeight( size );

            switch( childFit.getType() )
            {
                case FitExtent::Type::MATCH_PARENT:
                    weightSum += slot.weight;
                    break;


                case FitExtent::Type::FIXED:
                    fixedSize += childFit.getValue();
                    slot.isFixed = true;
                    slot.size = childFit.getValue();
                    break;
            }
        }

        for( auto& slot : m_slots )
        {
            auto childSize = size;

            auto const dirDim = m_direction == Direction::HORIZONTAL ? 0 : 1;

            if( slot.isFixed )
            {
                childSize( dirDim ) = slot.size;
            }
            else
            {
                childSize( dirDim ) = ( size( dirDim ) - fixedSize ) * slot.weight / weightSum;
            }

            slot.child->setSize( slot.child->finalFit( childSize ) );
        }
    }

    void Linear::setPosition( Position const& value )
    {
        Element::setPosition( value );

        auto childOffset = getPosition();

        auto const dirDim = m_direction == Direction::HORIZONTAL ? 0 : 1;

        for( auto const& slot : m_slots )
        {
            slot.child->setPosition( childOffset );
            childOffset( dirDim ) += slot.size;
        }
    }

    void Linear::update( float timeDiff )
    {
        for( auto& slot : m_slots )
        {
            if( slot.child )
            {
                slot.child->update( timeDiff );
            }
        }
    }

    void Linear::render( ttb::State& state ) const
    {
        for( auto const& slot : m_slots )
        {
            if( slot.child )
            {
                slot.child->render( state );
            }
        }
    }

    bool Linear::onEvent( Event const& event )
    {
        return std::any_of( std::begin( m_slots ),
                            std::end( m_slots ),
                            [ &event ]( auto const& slot )
                            {
                                return slot.child && slot.child->onEvent( event );
                            } );
    }

    void Linear::onChildChanged( Element& /* child */ )
    {
        changed();
    }

    void Linear::remove( Element& child )
    {
        auto const newEnd = std::remove_if( std::begin( m_slots ),
                                            std::end( m_slots ),
                                            [ &child ]( auto const& slot )
                                            {
                                                return slot.child == &child;
                                            } );

        if( newEnd == std::end( m_slots ) )
        {
            return;
        }

        child.setParent( nullptr );

        m_slots.erase( newEnd, std::end( m_slots ) );
        changed();
    }

    void Linear::insert( std::size_t position, Element* child, float weight )
    {
        child->setParent( this );
        m_slots.insert(
            std::next( std::begin( m_slots ) ), position, Slot{ false, weight, 0.0f, child } );
    }

    void Linear::add( Element* child, float weight )
    {
        child->setParent( this );
        m_slots.push_back( Slot{ false, weight, 0.0f, child } );
    }
}
