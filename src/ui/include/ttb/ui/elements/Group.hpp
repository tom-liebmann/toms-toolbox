#pragma once

#include <ttb/ui/CombinedElement.hpp>


namespace ttb::ui
{
    class Group : public CombinedElement
    {
    public:
        Group( Framework& framework );

        void addChild( Element* child, bool considerSize = true );
    };
}
