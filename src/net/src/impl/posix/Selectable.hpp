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
            Selectable();

            virtual ~Selectable();

            void selector( NetSelector* selector );

            virtual int handle() const = 0;

            // Indicate whether the selectable should be checked for readability/writability or not
            virtual bool checkRead() const = 0;
            virtual bool checkWrite() const = 0;

            // Are called by the passive selector to signal handle availability
            virtual void doRead() = 0;
            virtual void doWrite() = 0;

            // Is called by the writer thread to perform writing operations
            /**
             * \return Whether the writing process is done
             */
            virtual bool writeData() = 0;

        protected:
            NetSelector* selector();

        private:
            NetSelector* m_selector;
        };
    }
}
