#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class Ratio : public WrappedElement
    {
    public:
        Ratio( Framework& framework, float ratio = 1.0f );

        void child( Element* element );

        void ratio( float ratio );

        virtual Size fit( Size const& size ) override;

    private:
        float m_ratio;
    };
}
