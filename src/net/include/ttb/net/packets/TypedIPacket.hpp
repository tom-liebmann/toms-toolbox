#pragma once

#include <ttb/net/packets/IPacket.hpp>

#include <memory>


namespace ttb
{
    template < typename TType >
    class TypedIPacket : public ttb::IPacket
    {
    public:
        TypedIPacket( std::shared_ptr< ttb::IPacket const > const& parent );

        TType type() const;

        // Override: ttb::IPacket
        virtual void const* data() const override;
        virtual size_t size() const override;

    private:
        std::shared_ptr< ttb::IPacket const > m_parent;
    };
}


namespace ttb
{
    template < typename TType >
    TypedIPacket< TType >::TypedIPacket( std::shared_ptr< ttb::IPacket const > const& parent )
        : m_parent( parent )
    {
    }

    template < typename TType >
    TType TypedIPacket< TType >::type() const
    {
        return *reinterpret_cast< TType const* >( m_parent->data() );
    }

    template < typename TType >
    void const* TypedIPacket< TType >::data() const
    {
        return reinterpret_cast< uint8_t const* >( m_parent->data() ) + sizeof( TType );
    }

    template < typename TType >
    size_t TypedIPacket< TType >::size() const
    {
        return m_parent->size() - sizeof( TType );
    }
}
