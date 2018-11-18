#pragma once


namespace ttb
{
    template < typename TOutputType, typename TInputType >
    void connect( PushOutput< TOutputType >& output, PushInput< TInputType >& input );
}


namespace ttb
{
    template < typename TOutputType, typename TInputType >
    void connect( PushOutput< TOutputType >& output, PushInput< TInputType >& input )
    {
        output.inputSlot( input.m_slot );
    }
}
