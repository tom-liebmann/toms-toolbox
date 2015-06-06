#pragma once

namespace tg
{
    template< typename T, size_t... D >
    class Tensor
    {
    public:
    private:
        std::array< T, tg_private::pack_product< D... >::value() > m_values;
    };
}
