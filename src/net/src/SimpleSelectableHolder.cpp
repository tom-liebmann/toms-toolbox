#include <ttb/net/SimpleSelectableHolder.hpp>


namespace ttb
{
    SimpleSelectableHolder::SimpleSelectableHolder( std::shared_ptr< Selectable > selectable )
        : m_selectable( std::move( selectable ) )
    {
    }

    std::shared_ptr< Selectable > SimpleSelectableHolder::selectable() const
    {
        return m_selectable;
    }
}
