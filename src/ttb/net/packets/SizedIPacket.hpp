#pragma once

#include <ttb/net/Endianess.hpp>
#include <ttb/net/IPacket.hpp>

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
        SizedIPacket( TCPSocket& socket, Endianess endianess = Endianess::LITTLE );

        // IPacket
        virtual const uint8_t* getData() const override;
        virtual size_t getSize() const override;
        virtual std::string getContent( const std::string& inner ) const override;

    private:
        std::vector< uint8_t > m_data;
        Endianess m_endianess;
    };
}
