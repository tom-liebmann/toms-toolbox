#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>

#include <queue>


namespace ttb
{
    class Selectable;
}


namespace ttb
{
    class Selector
    {
    public:
        static std::unique_ptr< Selector > create();

        virtual ~Selector();

        virtual void interrupt() = 0;

        virtual void process( bool blocking = true ) = 0;

        virtual void processWrites( bool blocking = true ) = 0;

        virtual void add( std::shared_ptr< Selectable > const& socket ) = 0;

        virtual void remove( Selectable& socket ) = 0;
    };
}
