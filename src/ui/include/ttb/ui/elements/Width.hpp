#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/elements/Flex.hpp>


namespace ttb::ui
{
    class Width : public WrappedElement
    {
    public:
        Width( Framework& framework, float width );

        void child( Element* element );

    private:
        Flex m_flex;
    };
}
