#pragma once

#include <ttb/net/Selectable.hpp>

#include <memory>


namespace ttb
{
    class Listener : public Selectable
    {
    public:
        static std::shared_ptr< Listener > create( uint16_t port );

        virtual ~Listener();

        uint16_t port() const;

    protected:
        Listener( uint16_t port );

    private:
        uint16_t m_port;
    };
}
