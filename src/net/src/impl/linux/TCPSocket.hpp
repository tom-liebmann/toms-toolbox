#include <ttb/net/TCPSocket.hpp>


namespace ttb
{
    namespace emscripten
    {
        class TCPSocket : public ttb::TCPSocket
        {
        public:
            TCPSocket( std::string const& address, uint16_t port );

            TCPSocket( int handle );

            ~TCPSocket();

            // Override: ttb::TCPSocket
            virtual void send( void const* data, size_t size ) const override;
            virtual void receive( void* data, size_t size ) const override;

        private:
            int m_handle;
        };
    }
}
