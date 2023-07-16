/*
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>


namespace ttb
{
    namespace base64
    {
        std::string const characters =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        template < typename TIterator >
        std::string encode( TIterator first, TIterator pastLast );

        std::vector< uint8_t > decode( std::string const& value );
    }
}


namespace ttb
{
    namespace base64
    {
        template < typename TIterator >
        std::string encode( TIterator first, TIterator pastLast )
        {
            std::ostringstream stream;
            int i = 0;
            unsigned char char_array_3[ 3 ];
            unsigned char char_array_4[ 4 ];

            while( first != pastLast )
            {
                char_array_3[ i++ ] = *first;
                ++first;

                if( i == 3 )
                {
                    char_array_4[ 0 ] = ( char_array_3[ 0 ] & 0xfc ) >> 2;
                    char_array_4[ 1 ] = ( ( char_array_3[ 0 ] & 0x03 ) << 4 ) +
                                        ( ( char_array_3[ 1 ] & 0xf0 ) >> 4 );
                    char_array_4[ 2 ] = ( ( char_array_3[ 1 ] & 0x0f ) << 2 ) +
                                        ( ( char_array_3[ 2 ] & 0xc0 ) >> 6 );
                    char_array_4[ 3 ] = char_array_3[ 2 ] & 0x3f;

                    for( i = 0; i < 4; ++i )
                    {
                        stream << characters[ char_array_4[ i ] ];
                    }

                    i = 0;
                }
            }

            if( i )
            {
                for( int j = i; j < 3; ++j )
                {
                    char_array_3[ j ] = '\0';
                }

                char_array_4[ 0 ] = ( char_array_3[ 0 ] & 0xfc ) >> 2;
                char_array_4[ 1 ] =
                    ( ( char_array_3[ 0 ] & 0x03 ) << 4 ) + ( ( char_array_3[ 1 ] & 0xf0 ) >> 4 );
                char_array_4[ 2 ] =
                    ( ( char_array_3[ 1 ] & 0x0f ) << 2 ) + ( ( char_array_3[ 2 ] & 0xc0 ) >> 6 );
                char_array_4[ 3 ] = char_array_3[ 2 ] & 0x3f;

                for( int j = 0; j < i + 1; ++j )
                {
                    stream << characters[ char_array_4[ j ] ];
                }

                while( i++ < 3 )
                {
                    stream << '=';
                }
            }

            return stream.str();
        }
    }
}
