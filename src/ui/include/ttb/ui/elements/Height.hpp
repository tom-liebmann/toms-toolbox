#pragma once

#include <ttb/ui/WrappedElement.hpp>
#include <ttb/ui/elements/Flex.hpp>


namespace ttb::ui
{
    class Height : public WrappedElement
    {
    public:
        Height( Framework& framework, float height );

        void child( Element* element );

    private:
        Flex m_flex;
    };
}
