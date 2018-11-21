#include <ttb/utils/signal.hpp>

#include <iostream>


int main()
{
    auto add = []( int lhs, int rhs ) { return lhs + rhs; };

    using Signature = int( int, int );

    ttb::Signal< Signature > signal;
    ttb::Slot< Signature > slot( add );

    ttb::connect( signal, slot );

    for( int i = 0; i < 5; ++i )
    {
        std::cout << "Result: " << signal.call( i, 2 * i ) << '\n';
    }

    return EXIT_SUCCESS;
}
