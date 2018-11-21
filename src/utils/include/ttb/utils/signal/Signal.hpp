#pragma once

#include "priv/SignalConnector.hpp"

#include <iostream>
#include <memory>
#include <mutex>


namespace ttb
{
    template < typename TSignature >
    class Slot;
}


namespace ttb
{
    template < typename TSignature >
    class Signal;

    template < typename TReturn, typename... TArgs >
    class Signal< TReturn( TArgs... ) >
    {
    public:
        void disconnect();

        template < typename... TArgs2 >
        TReturn call( TArgs2&&... args );

    private:
        template < typename TReturn2, typename... TArgs2 >
        void slotConnector(
            std::shared_ptr< priv::SlotConnector< TReturn2( TArgs2... ) > > connector );

        std::mutex m_mutex;
        std::shared_ptr< priv::SignalConnector< TReturn( TArgs... ) > > m_connector;

        template < typename TInSignature, typename TOutSignature >
        friend void connect( Signal< TInSignature >& signal, Slot< TOutSignature > const& slot );
    };
}


namespace ttb
{
    template < typename TReturn, typename... TArgs >
    void Signal< TReturn( TArgs... ) >::disconnect()
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_connector.reset();
    }

    template < typename TReturn, typename... TArgs >
    template < typename... TArgs2 >
    TReturn Signal< TReturn( TArgs... ) >::call( TArgs2&&... args )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        if( m_connector )
        {
            auto connector = m_connector;
            lock.unlock();

            return connector->call( std::forward< TArgs2 >( args )... );
        }
        else
        {
            if constexpr( !std::is_same_v< void, TReturn > )
            {
                throw std::runtime_error( "Signal not connected" );
            }
        }
    }

    template < typename TReturn, typename... TArgs >
    template < typename TReturn2, typename... TArgs2 >
    void Signal< TReturn( TArgs... ) >::slotConnector(
        std::shared_ptr< priv::SlotConnector< TReturn2( TArgs2... ) > > connector )
    {
        std::lock_guard< std::mutex > lock( m_mutex );

        if( connector )
        {
            m_connector = std::make_shared<
                priv::SignalConnectorImpl< TReturn( TArgs... ), TReturn2( TArgs2... ) > >(
                std::move( connector ) );
        }
        else
        {
            m_connector.reset();
        }
    }
}
