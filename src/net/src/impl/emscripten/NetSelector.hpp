#pragma once

#include "TCPSocket.hpp"
#include <ttb/net/NetSelector.hpp>

#include <mutex>


namespace ttb
{
    namespace emscripten
    {
        class NetSelector : public ttb::NetSelector
        {
        public:
            NetSelector();

            ~NetSelector();

            virtual void add( std::shared_ptr< ttb::TCPSocket > socket ) override;

            virtual void remove( std::shared_ptr< ttb::TCPSocket > socket ) override;

            virtual std::shared_ptr< Provider< SlotType::ACTIVE, Event& > >
                eventOutput() const override;

            virtual void update() override;

        private:
            enum class ChangeType
            {
                ADD,
                REMOVE
            };

            std::shared_ptr< SimpleProvider< SlotType::ACTIVE, Event& > > m_eventOutput;

            std::queue< std::pair< ChangeType, std::shared_ptr< ttb::emscripten::TCPSocket > > >
                m_changes;

            std::vector< std::shared_ptr< ttb::emscripten::TCPSocket > > m_sockets;

            mutable std::mutex m_mutex;
        };
    }
}
