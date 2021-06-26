#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class Margin : public WrappedElement
    {
    public:
        Margin( Framework& framework, float right, float top, float left, float bottom );

        void child( Element* element );

        using WrappedElement::range;

        virtual void range( Range const& range ) override;

        virtual Range fit( Range const& space ) override;

    private:
        float m_right;
        float m_top;
        float m_left;
        float m_bottom;
    };
}
