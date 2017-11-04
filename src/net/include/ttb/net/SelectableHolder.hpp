#pragma once

#include <memory>


namespace ttb
{
    class Selectable;
}


namespace ttb
{
    class SelectableHolder
    {
    public:
        virtual ~SelectableHolder();

        virtual std::shared_ptr< Selectable > selectable() const = 0;
    };
}
