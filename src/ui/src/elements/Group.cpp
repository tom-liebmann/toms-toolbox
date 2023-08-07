#include <ttb/ui/elements/Group.hpp>

#include <ttb/math/matrix_operations.hpp>
#include <ttb/math/vector_operations.hpp>
#include <ttb/ui/XmlLoader.hpp>

#include <numeric>


namespace ttb::ui
{
    Group::Group( Root& root ) : Element{ root }
    {
        setWidth( Extent::Type::MATCH_CHILD );
        setHeight( Extent::Type::MATCH_CHILD );
    }

    Group::Group( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : Element{ root, node, loader }
    {
        for( auto child = node.first_node(); child; child = child->next_sibling() )
        {
            auto element = loader.loadElement( root, *child );
            auto considerSize = true;
            if( auto const value = loader.attrValue( *child, "consider_size" ) )
            {
                considerSize = ( value == "true" );
            }

            add( element, considerSize );
        }
    }

    float Group::fitWidth( float space ) const
    {
        if( getWidth().getType() == Extent::Type::MATCH_CHILD )
        {
            return std::accumulate( std::begin( m_children ),
                                    std::end( m_children ),
                                    0.0f,
                                    [ &space, this ]( auto const& maxSize, auto const& child )
                                    {
                                        if( child.considerSize )
                                        {
                                            return std::max( maxSize,
                                                             child.element->fitWidth(
                                                                 space - getLeft() - getRight() ) );
                                        }

                                        return maxSize;
                                    } ) +
                   getLeft() + getRight();
        }

        return Element::fitWidth( space );
    }

    float Group::fitHeight( float space ) const
    {
        if( getHeight().getType() == Extent::Type::MATCH_CHILD )
        {
            return std::accumulate( std::begin( m_children ),
                                    std::end( m_children ),
                                    0.0f,
                                    [ &space, this ]( auto const& maxSize, auto const& child )
                                    {
                                        if( child.considerSize )
                                        {
                                            return std::max( maxSize,
                                                             child.element->fitHeight(
                                                                 space - getTop() - getBottom() ) );
                                        }

                                        return maxSize;
                                    } ) +
                   getTop() + getBottom();
        }

        return Element::fitHeight( space );
    }

    void Group::setSize( Size const& value )
    {
        Element::setSize( value );

        for( auto const& child : m_children )
        {
            child.element->setSize( child.element->fit( getSize() ) );
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
