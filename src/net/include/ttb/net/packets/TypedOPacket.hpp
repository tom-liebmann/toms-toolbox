#pragma once

#include <ttb/net/TCPSocket.hpp>
#include <ttb/net/packets/OPacket.hpp>

#include <memory>


namespace ttb
{
    template < typename TType >
    class TypedOPacket : public ttb::OPacket
    {
    public:
        TypedOPacket( TType type, std::shared_ptr< ttb::OPacket const > const& packet );

        // Override: ttb::OPacket
        virtual size_t size() const override;
        virtual void send( ttb::PushOutput< std::vector< uint8_t > >& output ) const override;

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

    template < typename TType >
    void TypedOPacket< TType >::send( ttb::PushOutput< std::vector< uint8_t > >& output ) const
    {
    }
}
