#include <ttb/net/TCPSocket.hpp>


namespace ttb
{
    TCPSocket::TCPSocket()
        : m_dataInput( std::make_shared< DataInput >(
              [this]( auto data ) { this->onData( std::move( data ) ); } ) )
    {
    }

    TCPSocket::~TCPSocket() = default;

    TCPSocket::EventOutput& TCPSocket::eventOutput()
    {
        return m_eventOutput;
    }

    std::shared_ptr< TCPSocket::DataInput > const& TCPSocket::dataInput()
    {
        return m_dataInput;
    }
}
