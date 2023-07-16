#pragma once

#include <mutex>
#include <stdexcept>


namespace ttb
{
    template < typename TSignature >
    class Slot;
}


namespace ttb
{
    namespace priv
    {
        template < typename TSignature >
        class SlotConnector;

        template < typename TReturn, typename... TArgs >
        class SlotConnector< TReturn( TArgs... ) >
        {
        public:
            SlotConnector( Slot< TReturn( TArgs... ) >& slot );

            template < typename... TArgs2 >
            TReturn call( TArgs2&&... args );

        private:
            void disable();

            std::mutex m_mutex;

            Slot< TReturn( TArgs... ) >* m_slot;

            friend class Slot< TReturn( TArgs... ) >;
        };
    }
}


namespace ttb
{
    namespace priv
    {
        template < typename TReturn, typename... TArgs >
        SlotConnector< TReturn( TArgs... ) >::SlotConnector( Slot< TReturn( TArgs... ) >& slot )
            : m_slot( &slot )
        {
        }

        template < typename TReturn, typename... TArgs >
        template < typename... TArgs2 >
        TReturn SlotConnector< TReturn( TArgs... ) >::call( TArgs2&&... args )
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            if( m_slot )
            {
                auto callback = m_slot->m_callback;

                lock.unlock();

                return callback( std::forward< TArgs2 >( args )... );
            }
            else
            {
                if constexpr( !std::is_same_v< void, TReturn > )
                {
                    throw std::runtime_error( "Slot not connected" );
                }
            }
        }

        template < typename TReturn, typename... TArgs >
        void SlotConnector< TReturn( TArgs... ) >::disable()
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            m_slot = nullptr;
        }
    }
}
