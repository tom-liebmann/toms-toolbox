#pragma once

#include <tg/net/IPacket.hpp>
#include <tg/net/Endianess.hpp>

#include <vector>


// forward declarations
//=============================================================================

namespace tg
{
    class TCPSocket;
}



// declarations
//=============================================================================

namespace tg
{
    class SizedIPacket : public IPacket
    {
    public:
        SizedIPacket( TCPSocket& socket, Endianess endianess = Endianess::LITTLEENDIAN );

        // IPacket
        virtual const uint8_t* getData() const override;
        virtual size_t getSize() const override;
        virtual std::string getContent( const std::string& inner ) const override;

    private:
        std::vector< uint8_t > m_data;
        Endianess m_endianess;
    };
}
