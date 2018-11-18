#pragma once


namespace ttb
{
    template < typename TSignature >
    class Slot;

    template < typename TReturn, typename... TArgs >
    class Slot< TReturn( TArgs... ) >
    {
    public:
        using Callback = TReturn( TArgs... );

        Slot( Callback callback );

    private:
        Callback m_callback;
    };
}
