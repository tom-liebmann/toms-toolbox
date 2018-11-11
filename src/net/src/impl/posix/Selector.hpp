#pragma once

#include "Interruptor.hpp"
#include <ttb/net/Selector.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>


namespace ttb
{
    namespace posix
    {
        class Selectable;
    }
}


namespace ttb
{
    namespace posix
    {
        class Selector : public ttb::Selector
        {
        public:
            Selector();

            ~Selector();

            void startPolling( posix::Selectable& selectable );
            void startPolling( posix::Selectable& selectable,
                               std::unique_lock< std::mutex >& lock );

            // Notifies the selector to perform a write sweep
            void notifyWrite( posix::Selectable& selectable );
            void notifyRead( posix::Selectable& selectable );

            void remove( ttb::posix::Selectable& selectable,
                         std::unique_ptr< std::mutex >& selectableLock );

            // Override: Selector
            virtual void interruptWriter() override;
            virtual void interruptReader() override;
            virtual void processWrites( bool blocking ) override;
            virtual void processReads( bool blocking ) override;
            virtual void add( std::shared_ptr< ttb::Selectable > const& selectable ) override;
            virtual void remove( ttb::Selectable& selectable ) override;

        private:
            std::array< int, 2 > m_interruptHandles;

            std::mutex m_changeMutex;
            std::vector< size_t > m_freeSlots;
            std::vector< size_t > m_newFreeSlots;
            std::vector< std::shared_ptr< ttb::posix::Selectable > > m_selectables;

            int m_pollDescriptor;
            std::array< struct epoll_event, 10 > m_pollEvents;

            // We use two write queues to allow concurrent read/write operations.
            // The write thread just swaps the active queue by setting the currentWriteQueue index.
            std::mutex m_writeMutex;
            size_t m_currentWriteSet;
            std::array< std::unordered_set< posix::Selectable* >, 2 > m_writables;

            // Notify writer thread about a queue change
            std::condition_variable m_writeCondition;
        };
    }
}
