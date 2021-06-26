#include <ttb/ui/elements/Width.hpp>

#include <ttb/ui/elements/Flex.hpp>


namespace ttb::ui
{
    Width::Width( Framework& framework, float width )
        : WrappedElement{ framework }, m_flex{ framework, Flex::Direction::HORIZONTAL }
    {
        m_flex.addSlot( Flex::SlotType::FIXED, width );

        wrappedChild( &m_flex );
    }

    void Width::child( Element* element )
    {
        m_flex.child( 0, element );
    }
}
