#pragma once


namespace ttb
{
    class Interruptor : public Selectable
    {
    public:
        static std::shared_ptr< Interruptor > create();

        virtual ~Interruptor();

        virtual void interrupt() = 0;
    };
}


namespace ttb
{
    inline Interruptor::~Interruptor() = default;
}
