#pragma once

#include <ttb/utils/data_io/RandomAccessPacket.hpp>
#include <ttb/utils/data_io/Reader.hpp>


namespace ttb
{
    class BufferReader : public Reader
    {
    public:
        BufferReader( RandomAccessPacket const& packet );

        virtual bool read( void* buffer, std::size_t size ) override;

    private:
        RandomAccessPacket const& m_packet;

        std::size_t m_readHead{ 0 };
    };
}
