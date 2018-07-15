#include "Selectable.hpp"


namespace ttb
{
    namespace posix
    {
        Selectable::Selectable() : m_selector( nullptr )
        {
        }

        Selectable::~Selectable() = default;

        NetSelector* Selectable::selector()
        {
            return m_selector;
        }

        void Selectable::selector( NetSelector* selector )
        {
            m_selector = selector;
        }
    }
}
