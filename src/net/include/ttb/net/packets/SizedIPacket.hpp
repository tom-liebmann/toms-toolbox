#pragma once

#include <ttb/net/IPacket.hpp>
#include <ttb/utils/Endianness.hpp>

#include <vector>


// forward declarations
//=============================================================================

namespace ttb
{
    class TCPSocket;
}



// declarations
//=============================================================================

namespace ttb
{
    class SizedIPacket : public IPacket
    {
    public:
        SizedIPacket( TCPSocket& socket, Endianness endianness = Endianness::LITTLE );

        // IPacket
        virtual const uint8_t* getData() const override;
        virtual size_t getSize() const override;
        virtual std::string getContent( const std::string& inner ) const override;

    private:
        std::vector< uint8_t > m_data;
        Endianness m_endianness;
    };
}
