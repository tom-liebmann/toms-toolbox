#pragma once

#include "Packet.hpp"
#include "Writer.hpp"

#include <functional>


namespace ttb
{
    class PacketWriteHelper
    {
    public:
        PacketWriteHelper( size_t offset, Writer& writer );

        void write( void const* data, size_t dataSize );

        void write( ttb::Packet const& packet );

        size_t written() const;

    private:
        std::reference_wrapper< Writer > m_writer;
        intmax_t m_offset;
        size_t m_written{ 0 };
    };
}


namespace ttb
{
    inline PacketWriteHelper::PacketWriteHelper( size_t offset, Writer& writer )
        : m_writer{ writer }, m_offset{ static_cast< intmax_t >( offset ) }
    {
    }

    inline void PacketWriteHelper::write( void const* data, size_t dataSize )
    {
        if( m_offset < 0 )
        {
            return;
        }

        if( static_cast< size_t >( m_offset ) > dataSize )
        {
            m_offset -= dataSize;
            return;
        }

        auto const dataPtr = reinterpret_cast< uint8_t const* >( data );
        auto const written = m_writer.get().write( dataPtr + m_offset, dataSize - m_offset );
        m_written += written;
        m_offset += dataSize;
        m_offset -= written;
    }

    inline void PacketWriteHelper::write( ttb::Packet const& packet )
    {
        if( m_offset < 0 )
        {
            return;
        }

        auto const packetSize = packet.size();

        if( static_cast< size_t >( m_offset ) > packetSize )
        {
            m_offset -= packetSize;
            return;
        }

        auto const written = packet.write( static_cast< size_t >( m_offset ), m_writer.get() );
        m_written += written;
        m_offset += packetSize;
        m_offset -= written;
    }

    inline size_t PacketWriteHelper::written() const
    {
        return m_written;
    }
}
