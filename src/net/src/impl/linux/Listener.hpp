#pragma once

#undef linux

#include <ttb/net/Listener.hpp>


namespace ttb
{
    namespace linux
    {
        class Listener : public ttb::Listener
        {
        public:
            Listener( uint16_t port );

            ~Listener();

            // Override: Listener
            virtual std::unique_ptr< TCPSocket > accept() override;

        private:
            int m_handle;
        };
    }
}
