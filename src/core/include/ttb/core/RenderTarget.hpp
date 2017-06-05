#pragma once

#include <cstdlib>

// forward declarations
//=================================================================================================

namespace ttb
{
    class State;
}



// definitions
//=================================================================================================

namespace ttb
{
    class RenderTarget
    {
    public:
        virtual ~RenderTarget();

        virtual size_t width() const = 0;
        virtual size_t height() const = 0;

        virtual void begin( ttb::State& state ) const = 0;
        virtual void end( ttb::State& state ) const = 0;
    };
}
