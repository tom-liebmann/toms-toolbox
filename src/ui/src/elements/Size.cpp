#include <ttb/ui/elements/Size.hpp>


namespace ttb::ui
{
    Size::Size( Framework& framework, float width, float height )
        : WrappedElement{ framework }, m_width{ framework, width }, m_height{ framework, height }
    {
        m_width.child( &m_height );

        wrappedChild( &m_height );
    }

    void Size::child( Element* element )
    {
        m_height.child( element );
    }
}
