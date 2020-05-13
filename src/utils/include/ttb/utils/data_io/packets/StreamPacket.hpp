#pragma once

#include "../RandomAccessPacket.hpp"

#include <functional>


namespace ttb
{
    class StreamPacket : public RandomAccessPacket
    {
    public:
        StreamPacket( RandomAccessPacket const& packet );

        /**
         * Advances the read head.
         *
         * \param dstSize Number of bytes to advance the read head by
         * \return Pointer to previous data or nullptr if not enough data was left
         */
        virtual uint8_t const* pop( size_t dstSize );

        virtual size_t size() const override;
        virtual uint8_t const* data() const override;

    private:
        std::reference_wrapper< RandomAccessPacket const > m_packet;

        size_t m_readHead{ 0 };
    };
}
