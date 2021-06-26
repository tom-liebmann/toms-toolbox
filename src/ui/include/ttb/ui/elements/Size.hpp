#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/elements/Height.hpp>
#include <ttb/ui/elements/Width.hpp>


namespace ttb::ui
{
    class Size : public WrappedElement
    {
    public:
        Size( Framework& framework, float width, float height );

        void child( Element* element );

    private:
        Width m_width;
        Height m_height;
    };
}
