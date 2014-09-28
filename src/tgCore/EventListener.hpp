#pragma once

#include <memory>

namespace tgCore
{
    template< class T >
    class EventListener
    {
        public:
            virtual ~EventListener();

            virtual bool event(
                const std::unique_ptr< T >& event ) = 0;
    };

    template< class T >
    inline EventListener< T >::~EventListener()
    { }
}
