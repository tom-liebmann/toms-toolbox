#pragma once

#undef posix

#include "Selectable.hpp"

#include <ttb/net/Listener.hpp>


namespace ttb
{
    namespace posix
    {
        class Listener : public ttb::Listener,
                         public ttb::posix::Selectable,
                         public std::enable_shared_from_this< Listener >
        {
        public:
            Listener( uint16_t port );

            ~Listener();

            // Override: Selectable
            virtual int handle() const override;
            virtual bool isReadable() const override;
            virtual void doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) override;
            virtual bool isWritable() const override;
            virtual void
                doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) override;

        private:
            int m_handle;
        };
    }
}
