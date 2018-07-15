#pragma once

#include <ttb/net/packets/OPacket.hpp>

#include <ttb/net/DataWriter.hpp>


// declarations
//=================================================================================================

namespace ttb
{
    template < typename TPrefixType >
    class PrefixOPacket : public OPacket
    {
    public:
        template < typename TPrefixType2 >
        PrefixOPacket( TPrefixType2&& prefix, std::shared_ptr< OPacket const > payload );

        // Override: ttb::OPacket
        virtual size_t size() const override;
        virtual void write( DataWriter& writer ) const override;

    private:
        TPrefixType m_prefix;
        std::shared_ptr< OPacket const > m_payload;
    };
}


// definitions
//=================================================================================================

namespace ttb
{
    template < typename TPrefixType >
    template < typename TPrefixType2 >
    PrefixOPacket< TPrefixType >::PrefixOPacket( TPrefixType2&& prefix,
                                                 std::shared_ptr< OPacket const > payload )
        : m_prefix( std::forward< TPrefixType2 >( prefix ) ), m_payload( std::move( payload ) )
    {
    }

    template < typename TPrefixType >
    size_t PrefixOPacket< TPrefixType >::size() const
    {
        return sizeof( TPrefixType ) + m_payload->size();
    }

    template < typename TPrefixType >
    void PrefixOPacket< TPrefixType >::write( DataWriter& writer ) const
    {
        writer.write( &m_prefix, sizeof( TPrefixType ) );
        m_payload->write( writer );
    }
}
