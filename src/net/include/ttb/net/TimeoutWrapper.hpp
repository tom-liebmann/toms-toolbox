#pragma once

#include <ttb/net/PacketBridge.hpp>
#include <ttb/net/TCPSocket.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>


namespace ttb
{
    class TimeoutWrapper : public std::enable_shared_from_this< TimeoutWrapper >
    {
    public:
        using Duration = std::chrono::steady_clock::duration;
        using PacketInput = PacketBridge::PacketInput;
        using EventOutput = PacketBridge::EventOutput;

        static std::shared_ptr< TimeoutWrapper > create( std::shared_ptr< TCPSocket > socket,
                                                         Duration timeout );

        std::shared_ptr< PacketInput > const& packetInput();
        EventOutput& eventOutput();

        std::shared_ptr< TCPSocket > const& socket();

        std::shared_ptr< TCPSocket const > socket() const;

    private:
        TimeoutWrapper( std::shared_ptr< TCPSocket > socket, Duration timeout );

        void checkLoop();

        void onEventInput( ttb::Event& event );
        void onPacketInput( std::shared_ptr< ttb::OPacket const > packet );

        std::shared_ptr< PacketInput > m_packetInput;
        EventOutput m_eventOutput;

        std::shared_ptr< TCPSocket > m_socket;
        Duration m_timeout;
        PacketBridge m_packetBridge;

        bool m_ack;

        std::mutex m_mutex;
        std::condition_variable m_condition;
    };
}
