#pragma once

#include "../RandomAccessPacket.hpp"


namespace ttb
{
    template < typename TType, TType TValue >
    class ValuePacket : public RandomAccessPacket
    {
    public:
        virtual size_t size() const override;
        virtual uint8_t const* data() const override;

    private:
        static TType const s_value;
    };
}


namespace ttb
{
    template < typename TType, TType TValue >
    TType const ValuePacket< TType, TValue >::s_value = TValue;

    template < typename TType, TType TValue >
    size_t ValuePacket< TType, TValue >::size() const
    {
        return sizeof( TType );
    }

    template < typename TType, TType TValue >
    uint8_t const* ValuePacket< TType, TValue >::data() const
    {
        return reinterpret_cast< uint8_t const* >( &s_value );
    }
}
