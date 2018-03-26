#pragma once

#include <ttb/net/Selectable.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>

#include <memory>


namespace ttb
{
    class Listener : public Selectable
    {
    public:
        using EventOutput = ttb::PushOutput< Event& >;

        static std::shared_ptr< Listener > create( uint16_t port );

        virtual ~Listener();

        uint16_t port() const;

        EventOutput& eventOutput();

    protected:
        Listener( uint16_t port );

    private:
        uint16_t m_port;

        EventOutput m_eventOutput;
    };
}
