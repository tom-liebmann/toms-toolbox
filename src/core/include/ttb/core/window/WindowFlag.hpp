#pragma once

#include <cstdint>


namespace ttb
{
    enum class WindowFlag : uint32_t
    {
        NONE = 0,
        FULLSCREEN = 1 << 0,
        FLOATING = 1 << 1,
        RESIZABLE = 1 << 2,
        VSYNC = 1 << 3,
        HIDDEN = 1 << 4,
    };


    WindowFlag operator|( WindowFlag lhs, WindowFlag rhs );
}


namespace ttb
{
    inline WindowFlag operator|( WindowFlag lhs, WindowFlag rhs )
    {
        return static_cast< WindowFlag >( static_cast< uint32_t >( lhs ) |
                                          static_cast< uint32_t >( rhs ) );
    }
}
