#pragma once

#include <ttb/net/OPacket.hpp>
#include <ttb/net/TCPSocket.hpp>

#include <memory>


namespace ttb
{
    template < typename TType >
    class TypedOPacket : public ttb::OPacket
    {
    public:
        TypedOPacket( TType type, std::shared_ptr< ttb::OPacket const > const& packet );

        // Override: ttb::OPacket
        virtual void send( ttb::TCPSocket& socket ) const override;
        virtual size_t size() const override;

    private:
        TType m_type;
        std::shared_ptr< ttb::OPacket const > m_packet;
    };
}


namespace ttb
{
    template < typename TType >
    TypedOPacket< TType >::TypedOPacket( TType type,
                                         std::shared_ptr< ttb::OPacket const > const& packet )
        : m_type( type ), m_packet( packet )
    {
    }

    template < typename TType >
    void TypedOPacket< TType >::send( ttb::TCPSocket& socket ) const
    {
        socket.send( &m_type, sizeof( TType ) );
        m_packet->send( socket );
    }

    template < typename TType >
    size_t TypedOPacket< TType >::size() const
    {
        return m_packet->size() + sizeof( TType );
    }
}
