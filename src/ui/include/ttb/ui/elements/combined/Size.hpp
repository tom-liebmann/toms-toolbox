#pragma once

#include <ttb/ui/ElementWrapper.hpp>
#include <ttb/ui/elements/combined/Height.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class Size : public ElementWrapper, public UniqueCreate< Size >
    {
    public:
        void child( std::unique_ptr< Element > element );

    private:
        Size( Framework& framework, float width, float height );

        Height* m_heightElement;

        friend UniqueCreate;
    };
}
