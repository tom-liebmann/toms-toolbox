#pragma once

#undef posix

#include "Selectable.hpp"

#include <ttb/net/Interruptor.hpp>


namespace ttb
{
    namespace posix
    {
        class Interruptor : public ttb::Interruptor, public ttb::posix::Selectable
        {
        public:
            Interruptor();

            ~Interruptor();

            // Override: Interruptor
            virtual void interrupt() override;

            // Override: Selectable
            virtual int handle() const override;
            virtual bool checkRead() const override;
            virtual bool checkWrite() const override;
            virtual void doRead() override;
            virtual void doWrite() override;
            virtual bool writeData() override;

        private:
            int m_handles[ 2 ];
        };
    }
}
