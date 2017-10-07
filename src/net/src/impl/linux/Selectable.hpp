#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>


namespace ttb
{
    namespace linux
    {
        class Selectable
        {
        public:
            virtual ~Selectable();

            virtual int handle() const = 0;

            virtual bool isReadable() const = 0;
            virtual void doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) = 0;

            virtual bool isWritable() const = 0;
            virtual void doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) = 0;
        };
    }
}
