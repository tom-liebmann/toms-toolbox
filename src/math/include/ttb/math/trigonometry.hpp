#pragma once

// declarations
//=================================================================================================

namespace ttb
{
    template < typename TType >
    auto degToRad( TType degrees );
}



// definitions
//=================================================================================================

namespace ttb
{
    template < typename TType >
    auto degToRad( TType degrees )
    {
        return degrees / TType( 57.2957795130823208768 );
    }
}
