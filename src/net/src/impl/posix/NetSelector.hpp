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

            ~NetSelector();

            // Notifies the selector to perform a write sweep
            void notifyWrite();

            // Notifies the selector to reset the selection process
            void notifyChange();

            // Override: NetSelector
            virtual void add( std::shared_ptr< ttb::Selectable > const& selectable ) override;
            virtual void remove( ttb::Selectable& selectable ) override;

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

            void writeLoop();

            void selectLoop();

            std::mutex m_changeMutex;
            std::queue< Change > m_changes;

            std::shared_ptr< ttb::Interruptor > m_interruptor;

            std::mutex m_selectableMutex;
            std::vector< std::shared_ptr< ttb::posix::Selectable > > m_selectables;

            std::mutex m_mainMutex;
            bool m_running;
            std::condition_variable m_writeCondition;
            std::thread m_writeThread;
            std::thread m_selectThread;
        };
    }
}
