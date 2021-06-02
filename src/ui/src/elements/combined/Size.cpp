#include <ttb/ui/elements/combined/Size.hpp>

#include <ttb/ui/elements/combined/Height.hpp>
#include <ttb/ui/elements/combined/Width.hpp>


namespace ttb::ui
{
    void Size::child( std::unique_ptr< Element > element )
    {
        m_heightElement->child( std::move( element ) );
    }

    Size::Size( Framework& framework, float width, float height ) : ElementWrapper{ framework }
    {
        auto w = Width::create( framework, width );
        {
            auto h = Height::create( framework, height );
            m_heightElement = h.get();
        }

        wrappedChild( std::move( w ) );
    }
}
