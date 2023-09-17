#include <ttb/ui/elements/Linear.hpp>


namespace ttb::ui
{
    Linear::Linear( Root& root, Direction direction ) : Element{ root }, m_direction{ direction }
    {
        setWidth( m_direction == Direction::VERTICAL ? Extent::Type::MATCH_PARENT
                                                     : Extent::Type::MATCH_CHILD );

        setHeight( m_direction == Direction::VERTICAL ? Extent::Type::MATCH_CHILD
                                                      : Extent::Type::MATCH_PARENT );
    }

    Linear::Linear( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
    {
    }

    float Linear::fitWidth( float space ) const
    {
    }

    float Linear::fitHeight( float space ) const
    {
    }

    void Linear::setSize( Size const& value )
    {
        Element::setSize( value );

        auto const& size = getSize();

        auto weightSum = 0.0f;

        for( auto& slot : m_slots )
        {
            auto const childExtent = getExtent( slot.child, getDirDim() );

            switch( childExtent.getType() )
            {
                case Extent::Type::MATCH_PARENT:
                    weightSum += slot.weight;
                    break;

                case Extent::Type::MATCH_CHILD:
                case Extent::Type::FIXED:
                {
                    auto const childSize = fitChild( slot.child, getDirDim(), size( getDirDim() ) );
                    fixedSize += childSize;
                    slot.size = childSize;
                    break;
                }
            }
        }

        for( auto& slot : m_slots )
        {
            auto const childExtent = getExtent( slot.child, getDirDim() );

            if( childExtent.getType() == Extent::Type::MATCH_PARENT )
            {
                slot.size = size( getDirDim() ) * slot.weight / weightSum;
            }

            auto childSize = size;
            childSize( getDirDim() ) = slot.size;
            childSize( 1.0f - getDirDim() ) =
                fitChild( slot.child, 1 - getDirDim(), size( 1 - getDirDim() ) );
            slot.child->setSize( childSize );
        }
    }

    void Linear::setPosition( Position const& value )
    {
        Element::setPosition( value );

        auto childOffset = position;

        for( auto const& slot : m_slots )
        {
            slot.child->setPosition( childOffset );
            childOffset( getDirDim() ) += slot.size;
        }
    }

    void Linear::update( float timeDiff )
    {
    }

    void Linear::render( ttb::State& state ) const
    {
    }

    bool Linear::onEvent( Event const& event )
    {
    }

    void Linear::onChildChanged( Element& child )
    {
    }

    void Linear::remove( Element& child )
    {
    }

    void Linear::insert( std::size_t position, Element* child )
    {
    }

    void Linear::add( Element* child )
    {
    }

}
