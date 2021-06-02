#pragma once

#include <ttb/ui/ElementWrapper.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class Margin : public ElementWrapper, public UniqueCreate< Margin >
    {
    public:
        using ElementWrapper::range;

        void child( std::unique_ptr< Element > element );

        virtual void range( Range const& range ) override;
        virtual Range fit( Range const& space ) override;

    private:
        Margin( Framework& framework, float right, float top, float left, float bottom );

        float m_right;
        float m_top;
        float m_left;
        float m_bottom;

        friend UniqueCreate;
    };
}
