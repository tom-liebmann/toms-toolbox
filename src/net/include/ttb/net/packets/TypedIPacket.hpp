#pragma once

#include <ttb/net/packets/IPacket.hpp>


namespace ttb
{
    template < typename TType >
    class TypedIPacket : public IPacket
    {
    public:
        TypedIPacket( std::shared_ptr< IPacket > packet );

        TType type() const;

        // IPacket
        virtual void const* data() const override;
        virtual size_t size() const override;

    private:
        std::shared_ptr< IPacket > m_packet;
    };
}


namespace ttb
{
    template < typename TType >
    TypedIPacket< TType >::TypedIPacket( std::shared_ptr< IPacket > packet )
        : m_packet( std::move( packet ) )
    {
    }

    template < typename TType >
    TType TypedIPacket< TType >::type() const
    {
        return *reinterpret_cast< TType const* >( m_packet->data() );
    }

    template < typename TType >
    void const* TypedIPacket< TType >::data() const
    {
        return reinterpret_cast< uint8_t const* >( m_packet->data() ) + sizeof( TType );
    }

    template < typename TType >
    size_t TypedIPacket< TType >::size() const
    {
        return m_packet->size() - sizeof( TType );
    }
}
