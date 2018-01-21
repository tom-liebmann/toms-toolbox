#include "SendingHandshakeState.hpp"
#include "../WebSocket.hpp"
#include "ConnectedState.hpp"

#include <ttb/net/events.hpp>
#include <ttb/utils/SHA1.hpp>
#include <ttb/utils/base64.hpp>

#include <regex>
#include <sstream>
#include <sys/socket.h>


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            SendingHandshakeState::SendingHandshakeState( std::string handshakeMessage,
                                                          ttb::posix::WebSocket& socket )
                : ttb::posix::webSocket::SocketState( socket ), m_offset( 0 )
            {
                // Find Sec-WebSocket-Key in handshakeMessage
                std::string key = [&handshakeMessage]() -> std::string {
                    std::istringstream stream( handshakeMessage );
                    std::string line;

                    std::regex reg( "Sec-WebSocket-Key: ([A-Za-z0-9=+/]*)[\r\n]*" );
                    std::smatch match;

                    while( true )
                    {
                        std::getline( stream, line );

                        if( stream.eof() )
                        {
                            throw std::runtime_error( "Key not found in " + handshakeMessage );
                        }

                        if( std::regex_match( line, match, reg ) )
                        {
                            return match[ 1 ];
                        }
                    }
                }();

                ttb::SHA1 hasher;
                hasher.update( key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" );
                auto hash = hasher.finish();

                auto responseKey = ttb::base64::encode( std::begin( hash ), std::end( hash ) );

                std::ostringstream stream;

                stream << "HTTP/1.1 101 Switching Protocols\r\n"
                       << "Upgrade: websocket\r\n"
                       << "Connection: Upgrade\r\n"
                       << "Sec-WebSocket-Accept: " << responseKey << "\r\n"
                       << "Sec-WebSocket-Protocol: binary\r\n\r\n";

                m_message = stream.str();
            }

            bool SendingHandshakeState::isReadable() const
            {
                return false;
            }

            void SendingHandshakeState::doRead( std::shared_ptr< SelectableHolder > const& source,
                                                PushOutput< Event& >& eventOutput )
            {
            }

            bool SendingHandshakeState::isWritable() const
            {
                return true;
            }

            void SendingHandshakeState::doWrite( std::shared_ptr< SelectableHolder > const& source,
                                                 PushOutput< Event& >& eventOutput )
            {
                if( m_offset < m_message.length() )
                {
                    auto result = ::send( socket().handle(),
                                          m_message.c_str() + m_offset,
                                          m_message.length() - m_offset,
                                          MSG_NOSIGNAL );

                    if( result < 0 )
                    {
                        if( errno != EAGAIN && errno != EWOULDBLOCK )
                        {
                            ttb::events::BrokenConnection event( source );
                            eventOutput.push( event );
                            return;
                        }
                    }
                    else
                    {
                        m_offset += result;
                    }

                    if( m_offset == m_message.length() )
                    {
                        socket().state( std::make_unique< ConnectedState >( socket() ) );
                    }
                }
            }
        }
    }
}
