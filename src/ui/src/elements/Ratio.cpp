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
    Ratio::Ratio( Root& root, float ratio ) : Slot{ root }, m_ratio{ ratio }
    {
        setWidth( Extent::Type::MATCH_CHILD );
        setHeight( Extent::Type::MATCH_CHILD );
    }

    void Ratio::parseXml( XmlNode const& node, XmlLoader& loader )
    {
        if( auto const value = loader.attrValue( node, "value" ); value )
        {
            m_ratio = std::stof( std::string{ *value } );
        }

        Slot::parseXml( node, loader );
    }

    void Ratio::ratio( float ratio )
    {
        m_ratio = ratio;
        changed();
    }

    FitExtent Ratio::fitWidth( Size const& space ) const
    {
        auto const spaceM = getMargin().shrink( space );
        auto const ratio = spaceM( 0 ) / spaceM( 1 );
        return ratio > m_ratio ? space( 1 ) * m_ratio : space( 0 );
    }

    FitExtent Ratio::fitHeight( Size const& space ) const
    {
        auto const spaceM = getMargin().shrink( space );
        auto const ratio = spaceM( 0 ) / spaceM( 1 );
        return ratio > m_ratio ? space( 1 ) : space( 0 ) / m_ratio;
    }

    void Ratio::setSize( Size const& value )
    {
        Element::setSize( value );

        auto const& size = getSize();

        if( auto const child = getChild() )
        {
            auto const ratio = size( 0 ) / size( 1 );
            auto const childSize = ratio > m_ratio ? Size{ size( 1 ) * m_ratio, size( 1 ) }
                                                   : Size{ size( 0 ), size( 0 ) / m_ratio };
            child->setSize( childSize );
        }
    }
}
