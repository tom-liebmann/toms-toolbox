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
            virtual void add( std::shared_ptr< ttb::Selectable > const& selectable ) override;

            virtual void remove( std::shared_ptr< ttb::Selectable > const& selectable ) override;

            virtual void update( bool block ) override;

        private:
            enum class ChangeType
            {
                ADD,
                REMOVE
            };

            std::queue< std::pair< ChangeType, std::shared_ptr< ttb::posix::Selectable > > >
                m_changes;

            std::vector< std::shared_ptr< ttb::posix::Selectable > > m_selectables;

            mutable std::mutex m_mutex;
        };
    }
}
