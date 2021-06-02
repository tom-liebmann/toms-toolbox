#pragma once

#include <ttb/ui/ElementWrapper.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class Flex;
}


namespace ttb::ui
{
    class Height : public ElementWrapper, public UniqueCreate< Height >
    {
    public:
        void child( std::unique_ptr< Element > element );

    private:
        Height( Framework& framework, float height );

        Flex* m_flex;

        friend UniqueCreate;
    };
}
