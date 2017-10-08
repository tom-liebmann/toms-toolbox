/**
 * Source code taken from
 * https://github.com/vog/sha1

 * 100% Public Domain.
 * Original C Code
 * -- Steve Reid <steve@edmweb.com>
 * Small changes to fit into bglibs
 * -- Bruce Guenter <bruce@untroubled.org>
 * Translation to simpler C++ Code
 * -- Volker Grabsch <vog@notjusthosting.com>
 * Safety fixes
 * -- Eugene Hopkinson <slowriot at voxelstorm dot com>
 *
 * Modified to fit into ttb library
 */

#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <string>


namespace ttb
{
    class SHA1
    {
    public:
        SHA1();

        void update( std::string const& s );

        void update( std::istream& is );

        std::array< uint8_t, 20 > finish();

    private:
        uint32_t digest[ 5 ];
        std::string buffer;
        uint64_t transforms;
    };
}
