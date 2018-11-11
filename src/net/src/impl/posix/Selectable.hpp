#pragma once

#include <ttb/net/Selectable.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>


namespace ttb
{
    class SelectableHolder;

    namespace posix
    {
        class NetSelector;
    }
}


namespace ttb
{
    namespace posix
    {
        class Selectable : public ttb::Selectable
        {
        public:
            Selectable();

            virtual ~Selectable();

            virtual void process( int pollDescriptor ) = 0;

            virtual void processWrite() = 0;

            // Selectables are managed only in the selector thread. Since this is implementation
            // area, the user has no access to these attributes, which is why we can make them
            // public.

            // All changes to these attributes should be protected by the mutex
            std::mutex m_mutex;

            size_t m_slot;
            Selector* m_selector;     // The selector the selectable is currently assigned to
        };
    }
}
