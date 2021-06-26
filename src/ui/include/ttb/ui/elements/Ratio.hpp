#pragma once

#include <ttb/ui/WrappedElement.hpp>


namespace ttb::ui
{
    class Ratio : public WrappedElement
    {
    public:
        Ratio( Framework& framework, float ratio );

        void child( Element* element );

        virtual Range fit( Range const& space ) override;

    private:
        float m_ratio;
    };
}
