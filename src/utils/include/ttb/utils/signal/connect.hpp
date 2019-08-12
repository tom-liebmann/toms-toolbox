#pragma once


namespace ttb
{
    template < typename TInSignature, typename TOutSignature >
    void connect( Signal< TInSignature >& signal, Slot< TOutSignature > const& slot );

    template < typename TInSignature, typename TOutSignature >
    void connect( Slot< TInSignature > const& slot, Signal< TOutSignature >& signal );
}


namespace ttb
{
    template < typename TInSignature, typename TOutSignature >
    void connect( Signal< TInSignature >& signal, Slot< TOutSignature > const& slot )
    {
        signal.slotConnector( slot.m_connector );
    }

    template < typename TInSignature, typename TOutSignature >
    void connect( Slot< TInSignature > const& slot, Signal< TOutSignature >& signal )
    {
        connect( signal, slot );
    }
}
