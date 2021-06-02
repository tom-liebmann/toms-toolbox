#pragma once

#include <ttb/ui/ElementWrapper.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class Flex;
}


namespace ttb::ui
{
    class Width : public ElementWrapper, public UniqueCreate< Width >
    {
    public:
        void child( std::unique_ptr< Element > element );

    private:
        Width( Framework& framework, float width );

        Flex* m_flex;

        friend UniqueCreate;
    };
}
