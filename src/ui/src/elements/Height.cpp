#include <ttb/ui/elements/Height.hpp>


namespace ttb::ui
{
    Height::Height( Framework& framework, float height )
        : WrappedElement{ framework }, m_flex{ framework, Flex::Direction::VERTICAL }
    {
        m_flex.addSlot( Flex::SlotType::FIXED, height );

        wrappedChild( &m_flex );
    }

    void Height::child( Element* element )
    {
        m_flex.child( 0, element );
    }
}
