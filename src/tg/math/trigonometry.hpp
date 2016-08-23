#pragma once

// declarations
//=================================================================================================

namespace tg
{
    template < typename T >
    T degToRad( T degrees );
}



// definitions
//=================================================================================================

namespace tg
{
    template < typename T >
    T degToRad( T degrees )
    {
        return degrees / T( 57.2957795130823208768 );
    }
}
