#pragma once

#include <ttb/net/Selectable.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>


namespace ttb
{
    class SelectableHolder;

    namespace posix
    {
        class NetSelector;
    }
}


namespace ttb
{
    namespace posix
    {
        class Selectable : public ttb::Selectable
        {
        public:
            virtual ~Selectable();

            virtual void selector( ttb::posix::NetSelector* selector );

            virtual int handle() const = 0;

            virtual bool isReadable() const = 0;
            virtual void doRead() = 0;

            virtual bool isWritable() const = 0;
            virtual void doWrite() = 0;
        };
    }
}
