#pragma once

#include <ttb/net/Socket.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>

#include <string>
#include <vector>


namespace ttb
{
    class TCPSocket : public Selectable
    {
    public:
        using EventOutput = PushOutput< Event& >;
        using DataInput = PushInput< std::vector< uint8_t > >;

        static std::shared_ptr< TCPSocket > connect( std::string const& address, uint16_t port );

        TCPSocket();

        virtual ~TCPSocket();

        EventOutput& eventOutput();

        std::shared_ptr< DataInput > const& dataInput();

    private:
        virtual void onData( std::vector< uint8_t > data ) = 0;

        EventOutput m_eventOutput;
        std::shared_ptr< DataInput > m_dataInput;
    };
}
