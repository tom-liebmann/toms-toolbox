#pragma once

#include <ttb/net/SelectableHolder.hpp>


namespace ttb
{
    class SimpleSelectableHolder : public SelectableHolder
    {
    public:
        SimpleSelectableHolder( std::shared_ptr< Selectable > selectable );

        virtual std::shared_ptr< Selectable > selectable() const override;

    private:
        std::shared_ptr< Selectable > m_selectable;
    };
}
