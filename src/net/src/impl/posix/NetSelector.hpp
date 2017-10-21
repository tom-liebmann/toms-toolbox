#pragma once

#include "TCPSocket.hpp"
#include <ttb/net/NetSelector.hpp>

#include <mutex>


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
        class NetSelector : public ttb::NetSelector
        {
        public:
            NetSelector();

            ~NetSelector();

            virtual void add( std::shared_ptr< ttb::Selectable > const& socket ) override;

            virtual void remove( std::shared_ptr< ttb::Selectable > const& socket ) override;

            virtual std::shared_ptr< EventOutput > eventOutput() const override;

            virtual void update() override;

        private:
            enum class ChangeType
            {
                ADD,
                REMOVE
            };

            std::shared_ptr< SimpleProvider< SlotType::ACTIVE, Event& > > m_eventOutput;

            std::queue< std::pair< ChangeType, std::shared_ptr< ttb::posix::Selectable > > >
                m_changes;

            std::vector< std::shared_ptr< ttb::posix::Selectable > > m_sockets;

            mutable std::mutex m_mutex;
        };
    }
}
