#pragma once

#include "../RandomAccessPacket.hpp"

#include <functional>


namespace ttb
{
    class StreamPacket : public RandomAccessPacket
    {
    public:
        StreamPacket( RandomAccessPacket const& packet );

        virtual bool pop( void* dst, size_t dstSize );

        virtual size_t size() const override;
        virtual size_t write( size_t offset, size_t size, Writer& writer ) const override;

    private:
        std::reference_wrapper< RandomAccessPacket const > m_packet;

        size_t m_readHead{ 0 };
    };
}
