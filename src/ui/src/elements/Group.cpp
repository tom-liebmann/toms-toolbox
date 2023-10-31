#include <ttb/ui/elements/Group.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/XmlLoader.hpp>

#include <numeric>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Group >{ "group" };
    }
}


namespace ttb::ui
{
    Group::Group( Root& root ) : Element{ root }
    {
        setWidth( Extent::Type::MATCH_CHILD );
        setHeight( Extent::Type::MATCH_CHILD );
    }

    void Group::parseXml( XmlNode const& node, XmlLoader& loader )
    {
        Element::parseXml( node, loader );

        for( auto child = node.first_node(); child; child = child->next_sibling() )
        {
            auto element = loader.loadElement( getRoot(), *child );
            auto considerSize = true;
            if( auto const value = loader.attrValue( *child, "consider_size" ) )
            {
                considerSize = ( value == "true" );
            }

            add( element, considerSize );
        }
    }

    FitExtent Group::fitWidth( Size const& space ) const
    {
        if( getWidth().getType() != Extent::Type::MATCH_CHILD )
        {
            return Element::fitWidth( space );
        }

        auto const margin = getMargin();

        auto maxChildExtent = FitExtent{ 0.0f };

        for( auto const& child : m_children )
        {
            if( !child.considerSize )
            {
                continue;
            }

            auto const childFit = child.element->fitWidth(
                { space( 0 ) - margin.getRightLeft(), space( 1 ) - margin.getTopBottom() } );

            if( childFit.getType() == FitExtent::Type::MATCH_PARENT )
            {
                return childFit;
            }

            maxChildExtent =
                FitExtent{ std::max( maxChildExtent.getValue(), childFit.getValue() ) };
        }

        return { maxChildExtent.getValue() + margin.getRightLeft() };
    }

    FitExtent Group::fitHeight( Size const& space ) const
    {
        if( getHeight().getType() != Extent::Type::MATCH_CHILD )
        {
            return Element::fitHeight( space );
        }

        auto const margin = getMargin();

        auto maxChildExtent = FitExtent{ 0.0f };

        for( auto const& child : m_children )
        {
            if( !child.considerSize )
            {
                continue;
            }

            auto const childFit = child.element->fitHeight(
                { space( 0 ) - margin.getRightLeft(), space( 1 ) - margin.getTopBottom() } );

            if( childFit.getType() == FitExtent::Type::MATCH_PARENT )
            {
                return childFit;
            }

            maxChildExtent =
                FitExtent{ std::max( maxChildExtent.getValue(), childFit.getValue() ) };
        }

        return { maxChildExtent.getValue() + margin.getTopBottom() };
    }

    void Group::setSize( Size const& value )
    {
        Element::setSize( value );

        for( auto const& child : m_children )
        {
            child.element->setSize( child.element->finalFit( getSize() ) );
        }
    }

    void Group::setPosition( Position const& value )
    {
        Element::setPosition( value );

        for( auto const& child : m_children )
        {
            child.element->setPosition( getPosition() );
        }
    }

    void Group::update( float timeDiff )
    {
        for( auto const& child : m_children )
        {
            child.element->update( timeDiff );
        }
    }

    void Group::render( ttb::State& state ) const
    {
        for( auto const& child : m_children )
        {
            child.element->render( state );
        }
    }

    bool Group::onEvent( Event const& event )
    {
        return std::any_of( std::rbegin( m_children ),
                            std::rend( m_children ),
                            [ &event ]( auto const& child )
                            {
                                return child.element->onEvent( event );
                            } );
    }

    void Group::onChildChanged( Element& /* child */ )
    {
        changed();
    }

    void Group::remove( Element& child )
    {
        auto const newEnd = std::remove_if( std::begin( m_children ),
                                            std::end( m_children ),
                                            [ &child ]( auto const& slot )
                                            {
                                                return slot.element == &child;
                                            } );

        if( newEnd == std::end( m_children ) )
        {
            return;
        }

        child.setParent( nullptr );

        m_children.erase( newEnd, std::end( m_children ) );
        changed();
    }

    void Group::insert( std::size_t position, Element* child, bool considerSize )
    {
        child->setParent( this );
        m_children.insert( std::next( std::begin( m_children ), position ),
                           Slot{ considerSize, child } );
        changed();
    }

    void Group::add( Element* child, bool considerSize )
    {
        child->setParent( this );
        m_children.push_back( Slot{ considerSize, child } );
        changed();
    }
}
