#pragma once

#include <ttb/net/Selectable.hpp>
#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>


namespace ttb
{
    class SelectableHolder;
}


namespace ttb
{
    namespace posix
    {
        class Selectable : public ttb::Selectable
        {
        public:
            virtual ~Selectable();

            virtual int handle() const = 0;

            virtual bool isReadable() const = 0;
            virtual void doRead( std::shared_ptr< SelectableHolder > const& source,
                                 PushOutput< Event& >& eventOutput ) = 0;

            virtual bool isWritable() const = 0;
            virtual void doWrite( std::shared_ptr< SelectableHolder > const& source,
                                  PushOutput< Event& >& eventOutput ) = 0;
        };
    }
}
