#pragma once


namespace ttb
{
    template < typename TInSignature, typename TOutSignature >
    void connect( Signal< TInSignature >& signal, Slot< TOutSignature > const& slot );
}


namespace ttb
{
    template < typename TInSignature, typename TOutSignature >
    void connect( Signal< TInSignature >& signal, Slot< TOutSignature > const& slot )
    {
        signal.slotConnector( slot.m_connector );
    }
}
