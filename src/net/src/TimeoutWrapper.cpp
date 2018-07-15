#include <ttb/net/TimeoutWrapper.hpp>

#include <ttb/net/events.hpp>
#include <ttb/net/packets.hpp>


namespace
{
    enum class MessageType
    {
        PING,
        PONG,
        DATA
    };
}


namespace ttb
{
    TimeoutWrapper::TimeoutWrapper( std::shared_ptr< TCPSocket > socket, Duration timeout )
        : m_packetInput( std::make_shared< PacketInput >(
              [&]( auto packet ) { this->onPacketInput( std::move( packet ) ); } ) )
        , m_socket( std::move( socket ) )
        , m_timeout( timeout )
        , m_packetBridge( *m_socket )
        , m_thread( [this] { this->checkLoop(); } )
    {
        m_packetBridge.eventOutput().input( std::make_shared< ttb::PushInput< ttb::Event& > >(
            [this]( auto& e ) { this->onEventInput( e ); } ) );
    }

    TimeoutWrapper::~TimeoutWrapper()
    {
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            m_running = false;
        }

        m_condition.notify_all();

        m_thread.join();
    }

    std::shared_ptr< TimeoutWrapper::PacketInput > const& TimeoutWrapper::packetInput()
    {
        return m_packetInput;
    }

    TimeoutWrapper::EventOutput& TimeoutWrapper::eventOutput()
    {
        return m_eventOutput;
    }

    std::shared_ptr< TCPSocket > const& TimeoutWrapper::socket()
    {
        return m_socket;
    }

    std::shared_ptr< TCPSocket const > TimeoutWrapper::socket() const
    {
        return m_socket;
    }

    void TimeoutWrapper::checkLoop()
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        while( m_running )
        {
            switch( m_socket->connected() )
            {
                case TCPSocket::ConnectionState::CONNECTED:
                {
                    m_ack = false;
                    lock.unlock();

                    m_packetBridge.packetInput()->push(
                        ttb::DataOPacket::create().write( MessageType::PING ).finish() );

                    lock.lock();
                    break;
                }

                case TCPSocket::ConnectionState::CONNECTING:
                {
                    m_ack = false;
                    break;
                }

                case TCPSocket::ConnectionState::DISCONNECTED:
                {
                    m_ack = true;
                    break;
                }
            }

            m_condition.wait_for( lock, m_timeout );

            if( m_running && !m_ack )
            {
                m_socket->disconnect();
            }
        }
    }

    void TimeoutWrapper::onEventInput( ttb::Event& event )
    {
        switch( event.type() )
        {
            case ttb::events::SERVER_CONNECTION:
            case ttb::events::CLIENT_CONNECTION:
            {
                {
                    std::lock_guard< std::mutex > lock( m_mutex );
                    m_ack = true;
                }
                m_eventOutput.push( event );
                break;
            }

            case ttb::events::PACKET:
            {
                auto& e = static_cast< ttb::events::Packet& >( event );
                auto packet = std::make_unique< DataIPacket >( std::move( e.packet() ) );

                switch( packet->read< MessageType >() )
                {
                    case MessageType::PING:
                    {
                        m_packetBridge.packetInput()->push(
                            ttb::DataOPacket::create().write( MessageType::PONG ).finish() );
                        break;
                    }

                    case MessageType::PONG:
                    {
                        std::lock_guard< std::mutex > lock( m_mutex );
                        m_ack = true;
                        break;
                    }

                    case MessageType::DATA:
                    {
                        ttb::events::Packet ev( std::move( packet ) );
                        m_eventOutput.push( ev );
                        break;
                    }
                }

                break;
            }

            default:
                m_eventOutput.push( event );
        }
    }

    void TimeoutWrapper::onPacketInput( std::shared_ptr< ttb::OPacket const > packet )
    {
        m_packetBridge.packetInput()->push( std::make_shared< PrefixOPacket< MessageType > >(
            MessageType::DATA, std::move( packet ) ) );
    }
}
