#include <ttb/ui/elements/Size.hpp>


namespace ttb::ui
{
    Size::Size( Framework& framework, float width, float height )
        : WrappedElement{ framework }, m_width{ width }, m_height{ height }
    {
    }

    void Size::child( Element* element )
    {
        wrappedChild( element );
    }

    auto Size::fit( Element::Size const& /* size */ ) -> Element::Size
    {
        if( auto const child = wrappedChild(); child )
        {
            child->fit( { m_width, m_height } );
        }

        return Element::fit( { m_width, m_height } );
    }
}
