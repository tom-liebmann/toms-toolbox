#include <ttb/net/TimeoutWrapper.hpp>

#include <ttb/net/events.hpp>
#include <ttb/net/packets.hpp>

#include <iostream>


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
    class TimeoutWrapper::Thread : public std::enable_shared_from_this< Thread >
    {
    public:
        static std::shared_ptr< Thread > create( std::shared_ptr< TCPSocket > socket,
                                                 Duration timeout );

        std::shared_ptr< PacketInput > const& packetInput();

        EventOutput& eventOutput();

        void stop();

    private:
        Thread( std::shared_ptr< TCPSocket > socket, Duration timeout );

        void checkLoop();

        void onEventInput( ttb::Event& event );
        void onPacketInput( std::shared_ptr< ttb::OPacket const > packet );

        // External connections
        std::shared_ptr< PacketInput > m_packetInput;
        EventOutput m_eventOutput;

        // Internal connections
        using EventInput = ttb::PushInput< ttb::Event& >;
        std::shared_ptr< EventInput > m_eventInput;

        std::shared_ptr< TCPSocket > m_socket;
        Duration m_timeout;
        PacketBridge m_packetBridge;

        bool m_ack;

        bool m_running;

        std::mutex m_mutex;
        std::condition_variable m_condition;
    };
}


namespace ttb
{
    TimeoutWrapper::TimeoutWrapper( std::shared_ptr< TCPSocket > socket, Duration timeout )
        : m_socket( std::move( socket ) ), m_thread( Thread::create( m_socket, timeout ) )
    {
    }

    TimeoutWrapper::~TimeoutWrapper()
    {
        m_thread->stop();
    }

    std::shared_ptr< TimeoutWrapper::PacketInput > const& TimeoutWrapper::packetInput()
    {
        return m_thread->packetInput();
    }

    TimeoutWrapper::EventOutput& TimeoutWrapper::eventOutput()
    {
        return m_thread->eventOutput();
    }

    std::shared_ptr< TCPSocket > const& TimeoutWrapper::socket()
    {
        return m_socket;
    }

    std::shared_ptr< TCPSocket const > TimeoutWrapper::socket() const
    {
        return m_socket;
    }


    std::shared_ptr< TimeoutWrapper::Thread >
        TimeoutWrapper::Thread::create( std::shared_ptr< TCPSocket > socket, Duration timeout )
    {
        std::shared_ptr< Thread > result( new Thread( std::move( socket ), timeout ) );
        std::thread( [result] { result->checkLoop(); } ).detach();
        return result;
    }

    TimeoutWrapper::Thread::Thread( std::shared_ptr< TCPSocket > socket, Duration timeout )
        : m_packetInput( std::make_shared< PacketInput >(
              [&]( auto packet ) { this->onPacketInput( std::move( packet ) ); } ) )
        , m_eventInput(
              std::make_shared< EventInput >( [this]( auto& e ) { this->onEventInput( e ); } ) )
        , m_socket( std::move( socket ) )
        , m_timeout( timeout )
        , m_packetBridge( *m_socket )
        , m_running( true )
    {
        m_packetBridge.eventOutput().input( m_eventInput );
    }

    std::shared_ptr< TimeoutWrapper::PacketInput > const& TimeoutWrapper::Thread::packetInput()
    {
        return m_packetInput;
    }

    TimeoutWrapper::EventOutput& TimeoutWrapper::Thread::eventOutput()
    {
        return m_eventOutput;
    }

    void TimeoutWrapper::Thread::stop()
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_running = false;
    }

    void TimeoutWrapper::Thread::checkLoop()
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        auto self = shared_from_this();

        while( m_running )
        {
            switch( m_socket->connected() )
            {
                case TCPSocket::ConnectionState::CONNECTED:
                {
                    m_ack = false;
                    lock.unlock();

                    std::cout << "Time wrapper: Sending ping" << std::endl;
                    m_packetBridge.packetInput()->push(
                        ttb::DataOPacket::create().write( MessageType::PING ).finish() );

                    lock.lock();
                    break;
                }

                case TCPSocket::ConnectionState::CONNECTING:
                {
                    m_ack = false;
                    std::cout << "WAITING FOR CONNECTION" << std::endl;
                    break;
                }

                case TCPSocket::ConnectionState::DISCONNECTED:
                {
                    m_ack = true;
                    std::cout << "DISCONNECTED" << std::endl;
                    break;
                }
            }

            std::cout << "Time wrapper: waiting" << std::endl;
            m_condition.wait_for( lock, m_timeout );
            std::cout << "Time wrapper: done waiting" << std::endl;

            if( m_running && !m_ack )
            {
                std::cout << "Time wrapper: didn't get ack" << std::endl;

                auto socket = m_socket;
                lock.unlock();

                socket->disconnect();

                lock.lock();
            }
        }
    }

    void TimeoutWrapper::Thread::onEventInput( ttb::Event& event )
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

            case ttb::events::DISCONNECT:
            {
                m_packetBridge.reset();
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

    void TimeoutWrapper::Thread::onPacketInput( std::shared_ptr< ttb::OPacket const > packet )
    {
        m_packetBridge.packetInput()->push( std::make_shared< PrefixOPacket< MessageType > >(
            MessageType::DATA, std::move( packet ) ) );
    }
}
