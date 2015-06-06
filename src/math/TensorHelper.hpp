#pragma once

namespace tg_private
{
    template< size_t D0, size_t... D >
    class pack_product
    {
    public:
        static size_t value()
        {
            return D0 * pack_product< D... >::value();
        }
    };

    template<>
    class pack_product<>
    {
    public:
        static size_t value()
        {
            return 1;
        }
    };
}
