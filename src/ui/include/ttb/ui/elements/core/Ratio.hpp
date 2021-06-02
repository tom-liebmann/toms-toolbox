#pragma once

#include <ttb/ui/ElementWrapper.hpp>
#include <ttb/utils/UniqueCreate.hpp>


namespace ttb::ui
{
    class Ratio : public ElementWrapper, public UniqueCreate< Ratio >
    {
    public:
        void child( std::unique_ptr< Element > element );

        virtual Range fit( Range const& space ) override;

    private:
        Ratio( Framework& framework, float ratio );

        float m_ratio;

        friend UniqueCreate;
    };
}
