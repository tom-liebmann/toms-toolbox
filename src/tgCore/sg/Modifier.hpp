#pragma once

// forward declarations
//=============================================================================

namespace tg
{
    class State;
}



// declarations
//=============================================================================

namespace tg
{
    namespace sg
    {
        class Modifier
        {
            public:
                virtual ~Modifier();

                virtual uint32_t getPriority() const = 0;

                virtual void begin( State& state ) const = 0;
                virtual void end( State& state ) const = 0;
        };
    }
}



// definitions
//=============================================================================

namespace tg
{
    namespace sg
    {
        inline Modifier::~Modifier()
        { }
    }
}
