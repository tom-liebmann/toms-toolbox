#include <ttb/ui/elements/Ratio.hpp>

#include <ttb/ui/XmlFactory.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Ratio >{ "ratio" };
    }
}

namespace ttb::ui
{
    Ratio::Ratio( Root& root, float ratio ) : WrappedElement{ root }, m_ratio{ ratio }
    {
    }

    Ratio::Ratio( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ root }
    {
        if( auto const value = loader.attrValue( node, "value" ); value )
        {
            m_ratio = std::stof( std::string{ *value } );
        }

        if( auto child = node.first_node(); child )
        {
            wrappedChild( loader.loadElement( root, *child ) );
        }
    }

    void Ratio::ratio( float ratio )
    {
        m_ratio = ratio;
        changed();
    }

    void Ratio::child( Element* element )
    {
        wrappedChild( element );
    }

    auto Ratio::fit( Size const& size ) -> Size
    {
        auto const ratio = size( 0 ) / size( 1 );

        return ratio > m_ratio ? Size{ size( 1 ) * m_ratio, size( 1 ) }
                               : Size{ size( 0 ), size( 0 ) / m_ratio };
    }

    void Ratio::size( Size const& value )
    {
        Element::size( value );

        if( auto const child = wrappedChild(); child )
        {
            auto const ratio = value( 0 ) / value( 1 );
            auto const childSize = ratio > m_ratio ? Size{ value( 1 ) * m_ratio, value( 1 ) }
                                                   : Size{ value( 0 ), value( 0 ) / m_ratio };
            child->size( childSize );
        }
    }
}
