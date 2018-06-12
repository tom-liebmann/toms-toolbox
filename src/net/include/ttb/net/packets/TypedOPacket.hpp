#pragma once


namespace ttb
{
    template < typename TType >
    class TypedOPacket : public OPacket
    {
    public:
        TypedOPacket( TType type,
                      std::shared_ptr< OPacket const > packet,
                      Endianness endianness = Endianness::LITTLE );

        // OPacket
        virtual size_t size() const override;
        virtual void send( ttb::PushOutput< std::vector< uint8_t > >& output ) const override;

    private:
        TType m_type;
        std::shared_ptr< OPacket const > m_packet;
        Endianness m_endianness;
    };
}


namespace ttb
{
    template < typename TType >
    TypedOPacket< TType >::TypedOPacket( TType type,
                                         std::shared_ptr< OPacket const > packet,
                                         Endianness endianness )
        : m_type( type ), m_packet( std::move( packet ) ), m_endianness( endianness )
    {
    }

    template < typename TType >
    size_t TypedOPacket< TType >::size() const
    {
        return sizeof( TType ) + m_packet->size();
    }

    template < typename TType >
    void TypedOPacket< TType >::send( ttb::PushOutput< std::vector< uint8_t > >& output ) const
    {
        output.push( std::vector< uint8_t >( reinterpret_cast< uint8_t const* >( &m_type ),
                                             reinterpret_cast< uint8_t const* >( &m_type ) +
                                                 sizeof( TType ) ) );

        m_packet->send( output );
    }
}
