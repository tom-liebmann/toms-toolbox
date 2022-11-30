#include <ttb/ui/elements/Height.hpp>

#include <ttb/ui/XmlFactory.hpp>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Height >{ "height" };
    }
}



namespace ttb::ui
{
    Height::Height( Framework& framework, float height )
        : WrappedElement{ framework }, m_flex{ framework, Flex::Direction::VERTICAL }
    {
        m_flex.addSlot( Flex::SlotType::FIXED, height );

        wrappedChild( &m_flex );
    }

    Height::Height( Framework& framework, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ framework }, m_flex{ framework, Flex::Direction::VERTICAL }
    {
        if( auto const value = loader.attrValue( node, "value" ); value )
        {
            m_flex.addSlot( Flex::SlotType::FIXED, std::stof( std::string{ *value } ) );
        }

        wrappedChild( &m_flex );
    }

    void Height::child( Element* element )
    {
        m_flex.child( 0, element );
    }
}
