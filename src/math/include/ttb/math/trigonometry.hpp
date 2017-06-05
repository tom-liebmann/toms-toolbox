#pragma once

// declarations
//=================================================================================================

namespace ttb
{
    template < typename T >
    T degToRad( T degrees );
}



// definitions
//=================================================================================================

namespace ttb
{
    template < typename T >
    T degToRad( T degrees )
    {
        return degrees / T( 57.2957795130823208768 );
    }
}
