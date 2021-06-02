#pragma once

#include <ttb/ui/ElementWrapper.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class Center : public ElementWrapper, public UniqueCreate< Center >
    {
    public:
        enum class HAlignment
        {
            LEFT,
            CENTER,
            RIGHT,
        };

        enum class VAlignment
        {
            TOP,
            MIDDLE,
            BOTTOM,
        };

        void child( std::unique_ptr< Element > element );

        virtual void range( Range const& range ) override;
        virtual Range fit( Range const& space ) override;

    private:
        Center( Framework& framework, HAlignment hAlign, VAlignment vAlign );

        HAlignment m_hAlign;
        VAlignment m_vAlign;

        friend UniqueCreate;
    };
}
