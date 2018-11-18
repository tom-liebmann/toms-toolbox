#include <ttb/utils/dataIO.hpp>

#include <iostream>


int main()
{
    auto valueCallback = []( int value ) { std::cout << "Pushed value: " << value << '\n'; };

    using Signature = void( int );

    ttb::PushOutput< Signature > output1;
    ttb::PushInput< Signature > input1( valueCallback );

    ttb::connect( output1, input1 );

    for( int i = 0; i < 5; ++i )
    {
        output1.push( i );
    }

    return EXIT_SUCCESS;
}
