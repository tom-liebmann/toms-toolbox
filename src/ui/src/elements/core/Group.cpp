#include <ttb/ui/elements/core/Group.hpp>


namespace ttb::ui
{
    void Group::addChild( std::unique_ptr< Element > child, bool considerSize )
    {
        child->parent( *this );
        m_children.push_back( Slot{ considerSize, std::move( child ) } );
        changed();
    }

    void Group::range( Range const& range )
    {
        for( auto const& child : m_children )
        {
            auto const childRange = child.element->fit( range );
            child.element->range( { childRange.min() - range.min(), childRange.extent() } );
        }

        m_transform( 0, 2 ) = range.min( 0 );
        m_transform( 1, 2 ) = range.min( 1 );

        Element::range( range );
    }

    void Group::destroy()
    {
        for( auto const& child : m_children )
        {
            child.element->destroy();
        }
    }

    Group::Range Group::fit( Range const& space )
    {
        auto bound = Range{
            { std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity() },
            { -std::numeric_limits< float >::infinity(), -std::numeric_limits< float >::infinity() }
        };

        for( auto const& child : m_children )
        {
            if( child.considerSize )
            {
                bound.extend( child.element->fit( space ) );
            }
        }

        return bound;
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
        auto const u =
            state.uniform< ttb::Matrix< float, 3, 3 > >( "u_transform" ).push( m_transform );

        for( auto const& child : m_children )
        {
            child.element->render( state );
        }
    }

    bool Group::onEvent( Event& event )
    {
        for( auto const& child : m_children )
        {
            if( child.element->onEvent( event ) )
            {
                return true;
            }
        }

        return false;
    }

    Element* Group::child( std::string const& id )
    {
        size_t slotId = std::stoi( id );
        return m_children.at( slotId ).element.get();
    }

    Element const* Group::child( std::string const& id ) const
    {
        size_t slotId = std::stoi( id );
        return m_children.at( slotId ).element.get();
    }

    Group::Group( Framework& framework )
        : Element{ framework }, m_transform( ttb::mat::identity< float, 3 >() )
    {
    }
}
