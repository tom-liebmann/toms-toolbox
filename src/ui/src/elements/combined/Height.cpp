#include <ttb/ui/elements/combined/Height.hpp>

#include <ttb/ui/elements/core/Flex.hpp>


namespace ttb::ui
{
    void Height::child( std::unique_ptr< Element > element )
    {
        m_flex->modify().child( 0, std::move( element ) );
    }

    Height::Height( Framework& framework, float height ) : ElementWrapper{ framework }
    {
        auto flex = Flex::create( framework, Flex::Direction::VERTICAL );
        {
            auto flexMod = flex->modify();
            flexMod.slot( Flex::SlotType::FIXED, height );
        }
        m_flex = flex.get();
        wrappedChild( std::move( flex ) );
    }
}
