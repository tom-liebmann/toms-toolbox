#pragma once

#include <ttb/net/PacketBridge.hpp>
#include <ttb/net/TCPSocket.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>


namespace ttb
{
    class TimeoutWrapper
    {
    public:
        using Duration = std::chrono::steady_clock::duration;
        using PacketInput = PacketBridge::PacketInput;
        using EventOutput = PacketBridge::EventOutput;

        TimeoutWrapper( std::shared_ptr< TCPSocket > socket, Duration timeout );

        ~TimeoutWrapper();

        std::shared_ptr< PacketInput > const& packetInput();

        EventOutput& eventOutput();

        std::shared_ptr< TCPSocket > const& socket();

        std::shared_ptr< TCPSocket const > socket() const;

    private:
        class Thread;

        std::shared_ptr< TCPSocket > m_socket;

        std::shared_ptr< Thread > m_thread;
    };
}
