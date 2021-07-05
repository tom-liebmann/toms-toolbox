#pragma once

#include "Packet.hpp"
#include "Writer.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>


namespace ttb
{
    class PacketWriteHelper
    {
    public:
        PacketWriteHelper( size_t offset, size_t size, Writer& writer );

        void write( void const* data, size_t dataSize );

        void write( ttb::Packet const& packet );

        size_t written() const;

    private:
        std::reference_wrapper< Writer > m_writer;
        size_t m_offset;
        size_t m_size;
        size_t m_written{ 0 };
    };
}


namespace ttb
{
    inline PacketWriteHelper::PacketWriteHelper( size_t offset, size_t size, Writer& writer )
        : m_writer{ writer }, m_offset{ offset }, m_size{ size }
    {
    }

    inline void PacketWriteHelper::write( void const* data, size_t dataSize )
    {
        if( m_size == 0 )
        {
            // Done writing data
            return;
        }

        if( m_offset >= dataSize )
        {
            // Block not within writing area
            m_offset -= dataSize;
            return;
        }

        auto const writeSize = std::min( dataSize - m_offset, m_size );

        auto const dataPtr = reinterpret_cast< uint8_t const* >( data );
        auto const written = m_writer.get().write( dataPtr + m_offset, writeSize );
        m_written += written;
        m_size -= written;
        m_offset = 0;
    }

    inline void PacketWriteHelper::write( ttb::Packet const& packet )
    {
        if( m_size == 0 )
        {
            // Done writing data
            return;
        }

        // Only perform expensive packet size calculation once
        auto const packetSize = packet.size();

        if( m_offset >= packetSize )
        {
            // Block not within writing area
            m_offset -= packetSize;
            return;
        }

        auto const writeSize = std::min( packetSize - m_offset, m_size );
        auto const written = packet.write( m_offset, writeSize, m_writer.get() );

        m_written += written;
        m_size -= written;
        m_offset = 0;
    }

    inline size_t PacketWriteHelper::written() const
    {
        return m_written;
    }
}
