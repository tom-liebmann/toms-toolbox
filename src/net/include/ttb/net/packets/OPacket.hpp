#pragma once

#include <string>


// forward declarations
//=============================================================================

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

        /**
         * \returns Bytes written
         */
        virtual size_t write( DataWriter& writer, size_t offset, size_t size ) const = 0;
    };
}
