#include <ttb/ui/elements/combined/Width.hpp>

#include <ttb/ui/elements/core/Flex.hpp>


namespace ttb::ui
{
    void Width::child( std::unique_ptr< Element > element )
    {
        m_flex->modify().child( 0, std::move( element ) );
    }

    Width::Width( Framework& framework, float width ) : ElementWrapper{ framework }
    {
        auto flex = Flex::create( framework, Flex::Direction::HORIZONTAL );
        {
            auto flexMod = flex->modify();
            flexMod.slot( Flex::SlotType::FIXED, width );
        }
        m_flex = flex.get();
        wrappedChild( std::move( flex ) );
    }
}
