#pragma once

#include <string>
#include <vector>


// forward declarations
//=================================================================================================

namespace ttb
{
    class DataWriter;
}


// declarations
//=============================================================================

namespace ttb
{
    class OPacket
    {
    public:
        virtual ~OPacket();

        virtual size_t size() const = 0;
        virtual void write( DataWriter& writer ) const = 0;
    };
}
