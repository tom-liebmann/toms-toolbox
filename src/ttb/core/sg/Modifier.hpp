#pragma once

#include <cstdint>

// forward declarations
//=============================================================================

namespace ttb
{
    class State;
}



// declarations
//=============================================================================

namespace ttb
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

namespace ttb
{
    namespace sg
    {
        inline Modifier::~Modifier()
        {
        }
    }
}
