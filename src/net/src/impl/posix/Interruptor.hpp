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
            virtual bool isReadable() const override;
            virtual void doRead() override;
            virtual bool isWritable() const override;
            virtual void doWrite() override;

        private:
            int m_handles[ 2 ];
        };
    }
}
