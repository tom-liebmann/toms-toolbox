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

#include <ttb/utils/base64.hpp>


namespace
{
    bool is_base64( unsigned char c );
}


namespace ttb
{
    namespace base64
    {
        std::vector< uint8_t > decode( std::string const& value )
        {
            int in_len = static_cast< int >( value.size() );
            int i = 0;
            int in_ = 0;
            unsigned char char_array_4[ 4 ], char_array_3[ 3 ];
            std::vector< uint8_t > ret;

            while( in_len-- && ( value[ in_ ] != '=' ) && is_base64( value[ in_ ] ) )
            {
                char_array_4[ i++ ] = value[ in_ ];
                in_++;
                if( i == 4 )
                {
                    for( i = 0; i < 4; i++ )
                    {
                        char_array_4[ i ] =
                            static_cast< unsigned char >( characters.find( char_array_4[ i ] ) );
                    }

                    char_array_3[ 0 ] = static_cast< unsigned char >(
                        ( char_array_4[ 0 ] << 2 ) + ( ( char_array_4[ 1 ] & 0x30 ) >> 4 ) );
                    char_array_3[ 1 ] =
                        static_cast< unsigned char >( ( ( char_array_4[ 1 ] & 0xf ) << 4 ) +
                                                      ( ( char_array_4[ 2 ] & 0x3c ) >> 2 ) );
                    char_array_3[ 2 ] = static_cast< unsigned char >(
                        ( ( char_array_4[ 2 ] & 0x3 ) << 6 ) + char_array_4[ 3 ] );

                    for( i = 0; ( i < 3 ); i++ )
                    {
                        ret.push_back( char_array_3[ i ] );
                    }
                    i = 0;
                }
            }

            if( i )
            {
                for( int j = i; j < 4; ++j )
                {
                    char_array_4[ j ] = 0;
                }

                for( int j = 0; j < 4; ++j )
                {
                    char_array_4[ j ] =
                        static_cast< unsigned char >( characters.find( char_array_4[ j ] ) );
                }

                char_array_3[ 0 ] = static_cast< unsigned char >(
                    ( char_array_4[ 0 ] << 2 ) + ( ( char_array_4[ 1 ] & 0x30 ) >> 4 ) );
                char_array_3[ 1 ] = static_cast< unsigned char >(
                    ( ( char_array_4[ 1 ] & 0xf ) << 4 ) + ( ( char_array_4[ 2 ] & 0x3c ) >> 2 ) );
                char_array_3[ 2 ] = static_cast< unsigned char >(
                    ( ( char_array_4[ 2 ] & 0x3 ) << 6 ) + char_array_4[ 3 ] );

                for( int j = 0; j < i - 1; ++j )
                {
                    ret.push_back( char_array_3[ j ] );
                }
            }

            return ret;
        }
    }
}


namespace
{
    bool is_base64( unsigned char c )
    {
        return ( isalnum( c ) || ( c == '+' ) || ( c == '/' ) );
    }
}
