#pragma once

#undef posix

#include "Selectable.hpp"

#include <ttb/net/Listener.hpp>


namespace ttb
{
    namespace posix
    {
        class Listener : public ttb::Listener, public ttb::posix::Selectable
        {
        public:
            Listener( uint16_t port );

            ~Listener();

            // Override: Selectable
            virtual int handle() const override;
            virtual bool checkRead() const override;
            virtual bool checkWrite() const override;
            virtual void doRead() override;
            virtual void doWrite() override;
            virtual bool writeData() override;

        private:
            int m_handle;
        };
    }
}
