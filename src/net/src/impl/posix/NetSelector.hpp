#pragma once

#include "Interruptor.hpp"
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

            virtual void add( std::shared_ptr< ttb::Selectable > const& selectable ) override;

            virtual void remove( ttb::Selectable const& selectable ) override;

            virtual void update( bool block ) override;

        private:
            struct Change
            {
            public:
                enum class Type
                {
                    ADD,
                    REMOVE
                };

                Change( std::shared_ptr< ttb::posix::Selectable > data );

                Change( Selectable const& remove );

                ~Change();

                Type type() const;

                std::shared_ptr< ttb::posix::Selectable > const& dataAdd();

                Selectable const& dataRemove();

            private:
                Type m_type;

                union Data {
                    Data();

                    ~Data();

                    Selectable const* remove;
                    std::shared_ptr< ttb::posix::Selectable > add;
                } m_data;
            };

            std::queue< Change > m_changes;

            std::shared_ptr< ttb::Interruptor > m_interruptor;

            std::vector< std::shared_ptr< ttb::posix::Selectable > > m_selectables;

            mutable std::mutex m_mutex;
        };
    }
}
