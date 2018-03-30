#include <ttb/net/WebSocket.hpp>
#include <ttb/net/events.hpp>
#include <ttb/net/webSocket/ServerHandshakeState.hpp>
#include <ttb/utils/SHA1.hpp>
#include <ttb/utils/base64.hpp>

#include <regex>
#include <sstream>


namespace
{
    std::string findKey( std::string const& message );

    std::string buildResponseKey( std::string const& key );
}


namespace ttb
{
    namespace webSocket
    {
        ServerHandshakeState::ServerHandshakeState( ttb::WebSocket& socket ) : SocketState( socket )
        {
        }

        void ServerHandshakeState::onEventInput( ttb::Event& event )
        {
            auto& e = static_cast< ttb::events::Data& >( event );

            uint8_t buffer[ 256 ];

            auto result = e.read( buffer, 255 );

            m_data.insert( std::end( m_data ), buffer, buffer + result );

            if( m_data.size() >= 4 )
            {
                if( m_data[ m_data.size() - 4 ] == 13 && m_data[ m_data.size() - 3 ] == 10 &&
                    m_data[ m_data.size() - 2 ] == 13 && m_data[ m_data.size() - 1 ] == 10 )
                {
                    // Process response
                    std::string handshakeMessage( std::begin( m_data ), std::end( m_data ) );

                    auto key = findKey( handshakeMessage );

                    auto responseKey = buildResponseKey( key );

                    std::ostringstream stream;

                    stream << "HTTP/1.1 101 Switching Protocols\r\n"
                           << "Upgrade: websocket\r\n"
                           << "Connection: Upgrade\r\n"
                           << "Sec-WebSocket-Accept: " << responseKey << "\r\n"
                           << "Sec-WebSocket-Protocol: binary\r\n\r\n";

                    auto response = stream.str();

                    socket().dataOutput().push(
                        std::vector< uint8_t >( std::begin( response ), std::end( response ) ) );

                    // Switch to connected state
                    socket().state( std::make_unique< ConnectedState >( socket() ) );

                    // Send all data packages that were held back
                    for( auto data : m_sendBuffer )
                    {
                        socket().dataInput()->push( std::move( data ) );
                    }
                }
            }
        }

        void ServerHandshakeState::onDataInput( std::vector< uint8_t > data )
        {
            m_sendBuffer.push_back( std::move( data ) );
        }
    }
}


namespace
{
    std::string findKey( std::string const& message )
    {
        std::istringstream stream( message );
        std::string line;

        std::regex reg( "Sec-WebSocket-Key: ([A-Za-z0-9=+/]*)[\r\n]*" );
        std::smatch match;

        while( true )
        {
            std::getline( stream, line );

            if( stream.eof() )
            {
                throw std::runtime_error( "Key not found in " + message );
            }

            if( std::regex_match( line, match, reg ) )
            {
                return match[ 1 ];
            }
        }
    }

    std::string buildResponseKey( std::string const& key )
    {
        ttb::SHA1 hasher;
        hasher.update( key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" );
        auto hash = hasher.finish();

        return ttb::base64::encode( std::begin( hash ), std::end( hash ) );
    }
}
