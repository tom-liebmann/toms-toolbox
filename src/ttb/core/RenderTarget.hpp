#pragma once

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

        virtual void begin( ttb::State& state ) const = 0;
        virtual void end( ttb::State& state ) const = 0;
    };
}
