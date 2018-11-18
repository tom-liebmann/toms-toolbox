#pragma once

#include "priv/PushOutputSlot.hpp"

#include <iostream>
#include <memory>
#include <mutex>


namespace ttb
{
    template < typename TType >
    class PushInput;
}


namespace ttb
{
    template < typename TType >
    class PushOutput
    {
    public:
        void disconnect();

        /**
         * \return true if the data was successfully pushed
         */
        template < typename TType2 >
        bool push( TType2&& data );

    private:
        template < typename TType2 >
        void inputSlot( std::shared_ptr< priv::PushInputSlot< TType2 > > slot );

        std::mutex m_mutex;
        std::shared_ptr< priv::PushOutputSlot< TType > > m_slot;

        template < typename TOutputType, typename TInputType >
        friend void connect( PushOutput< TOutputType >&, PushInput< TInputType >& );
    };
}


namespace ttb
{
    template < typename TType >
    void PushOutput< TType >::disconnect()
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_slot.reset();
    }

    template < typename TType >
    template < typename TType2 >
    bool PushOutput< TType >::push( TType2&& data )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        if( m_slot )
        {
            auto slot = m_slot;
            lock.unlock();

            if( slot->push( std::forward< TType2 >( data ) ) )
            {
                return true;
            }
            else
            {
                disconnect();
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    template < typename TType >
    template < typename TType2 >
    void PushOutput< TType >::inputSlot( std::shared_ptr< priv::PushInputSlot< TType2 > > slot )
    {
        std::lock_guard< std::mutex > lock( m_mutex );

        if( slot )
        {
            m_slot =
                std::make_shared< priv::PushOutputSlotImpl< TType, TType2 > >( std::move( slot ) );
        }
        else
        {
            m_slot.reset();
        }
    }
}
