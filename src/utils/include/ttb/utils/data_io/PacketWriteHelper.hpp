#pragma once

#include "Packet.hpp"
#include "Writer.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>


namespace ttb
{
    class PacketWriteHelper
    {
    public:
        PacketWriteHelper( size_t offset, size_t size, Writer& writer );

        void write( void const* data, size_t dataSize );

        void write( ttb::Packet const& packet );

        void write( ttb::Packet const& packet, size_t offset, size_t size );

        template < typename TType >
        void write( TType const& value );

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

    inline void PacketWriteHelper::write( ttb::Packet const& packet, size_t offset, size_t size )
    {
        if( m_size == 0 )
        {
            // Done writing data
            return;
        }

        if( m_offset >= size )
        {
            // Block not within writing area
            m_offset -= size;
            return;
        }

        auto const writeSize = std::min( size - m_offset, m_size );
        auto const written = packet.write( m_offset + offset, writeSize, m_writer.get() );

        m_written += written;
        m_size -= written;
        m_offset = 0;
    }

    inline void PacketWriteHelper::write( ttb::Packet const& packet )
    {
        write( packet, 0, packet.size() );
    }

    template < typename TType >
    inline void PacketWriteHelper::write( TType const& value )
    {
        write( &value, sizeof( TType ) );
    }

    inline size_t PacketWriteHelper::written() const
    {
        return m_written;
    }
}
