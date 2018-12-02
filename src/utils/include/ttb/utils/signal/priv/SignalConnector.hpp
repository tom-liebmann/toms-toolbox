#pragma once

#include "SlotConnector.hpp"

#include <ttb/utils/type_conversion.hpp>

#include <memory>


namespace ttb
{
    namespace priv
    {
        template < typename TInSignature >
        class SignalConnector;

        template < typename TInReturn, typename... TInArgs >
        class SignalConnector< TInReturn( TInArgs... ) >
        {
        public:
            virtual ~SignalConnector();

            virtual TInReturn call( TInArgs... args ) = 0;
        };

        template < typename TInSignature, typename TOutSignature >
        class SignalConnectorImpl;

        template < typename TInReturn,
                   typename... TInArgs,
                   typename TOutReturn,
                   typename... TOutArgs >
        class SignalConnectorImpl< TInReturn( TInArgs... ), TOutReturn( TOutArgs... ) >
            : public SignalConnector< TInReturn( TInArgs... ) >
        {
        public:
            SignalConnectorImpl(
                std::shared_ptr< SlotConnector< TOutReturn( TOutArgs... ) > > slotConnector );

            virtual TInReturn call( TInArgs... args ) override;

        private:
            std::shared_ptr< SlotConnector< TOutReturn( TOutArgs... ) > > m_slotConnector;
        };
    }
}


namespace ttb
{
    namespace priv
    {
        template < typename TInReturn, typename... TInArgs >
        SignalConnector< TInReturn( TInArgs... ) >::~SignalConnector() = default;


        template < typename TInReturn,
                   typename... TInArgs,
                   typename TOutReturn,
                   typename... TOutArgs >
        SignalConnectorImpl< TInReturn( TInArgs... ), TOutReturn( TOutArgs... ) >::
            SignalConnectorImpl(
                std::shared_ptr< SlotConnector< TOutReturn( TOutArgs... ) > > slotConnector )
            : m_slotConnector( std::move( slotConnector ) )
        {
        }

        template < typename TInReturn,
                   typename... TInArgs,
                   typename TOutReturn,
                   typename... TOutArgs >
        TInReturn SignalConnectorImpl< TInReturn( TInArgs... ), TOutReturn( TOutArgs... ) >::call(
            TInArgs... args )
        {
            std::tuple< TInArgs... > argsTuple{ args... };

            if constexpr( std::is_same_v< void, TInReturn > )
            {
                call_converted< TOutArgs... >::call(
                    [this]( auto&&... innerArgs ) -> TOutReturn {
                        return m_slotConnector->call(
                            std::forward< decltype( innerArgs ) >( innerArgs )... );
                    },
                    argsTuple );
            }
            else
            {
                return call_converted< TOutArgs... >::call(
                    [this]( auto&&... innerArgs ) -> TOutReturn {
                        return m_slotConnector->call(
                            std::forward< decltype( innerArgs ) >( innerArgs )... );
                    },
                    argsTuple );
            }
        }
    }
}
