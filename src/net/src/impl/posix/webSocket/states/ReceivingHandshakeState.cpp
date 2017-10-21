#include "ReceivingHandshakeState.hpp"
#include "../WebSocket.hpp"
#include "SendingHandshakeState.hpp"

#include <ttb/net/netEvents.hpp>

#include <sys/socket.h>


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            ReceivingHandshakeState::ReceivingHandshakeState( ttb::posix::WebSocket& socket )
                : ttb::posix::webSocket::SocketState( socket )
            {
            }

            bool ReceivingHandshakeState::isReadable() const
            {
                return true;
            }

            void ReceivingHandshakeState::doRead(
                ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
            {
                uint8_t buffer[ 256 ];

                auto result = ::recv( socket().handle(), buffer, 255, MSG_DONTWAIT );

                if( result < 0 )
                {
                    if( errno != EAGAIN && errno != EWOULDBLOCK )
                    {
                        throw std::runtime_error( "Connection lost" );
                    }
                }
                else if( result == 0 )
                {
                    ttb::events::SocketClosedEvent event( socket().shared_from_this() );
                    eventOutput.push( event );
                    return;
                }
                else
                {
                    m_data.insert( std::end( m_data ), buffer, buffer + result );

                    if( m_data.size() >= 4 )
                    {
                        if( m_data[ m_data.size() - 4 ] == 13 &&
                            m_data[ m_data.size() - 3 ] == 10 &&
                            m_data[ m_data.size() - 2 ] == 13 && m_data[ m_data.size() - 1 ] == 10 )
                        {
                            socket().state( std::make_unique< SendingHandshakeState >(
                                std::string( std::begin( m_data ), std::end( m_data ) ),
                                socket() ) );
                        }
                    }
                }
            }

            bool ReceivingHandshakeState::isWritable() const
            {
                return false;
            }

            void ReceivingHandshakeState::doWrite(
                ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
            {
            }
        }
    }
}
