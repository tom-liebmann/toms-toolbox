#pragma once

namespace tgCore
{
    namespace sg
    {
        class Modifier
        {
            public:
                virtual ~Modifier();

                virtual uint32_t getPriority() const = 0;

                virtual void begin() const = 0;
                virtual void end() const = 0;
        };



        inline Modifier::~Modifier()
        { }
    }
}
