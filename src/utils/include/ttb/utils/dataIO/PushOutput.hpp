#pragma once

#include "helper/pushOutputHelper.hpp"

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
        template < typename TType2 >
        void input( std::shared_ptr< PushInput< TType2 > > const& input );

        /**
         * \return true if the data was successfully pushed
         */
        bool push( TType data );

    private:
        std::mutex m_mutex;
        std::shared_ptr< priv::PushOutputSlot< TType > > m_slot;
    };
}


namespace ttb
{
    template < typename TType >
    template < typename TType2 >
    void PushOutput< TType >::input( std::shared_ptr< PushInput< TType2 > > const& input )
    {
        std::lock_guard< std::mutex > lock( m_mutex );

        if( input )
        {
            m_slot = std::make_shared< priv::PushOutputSlotImpl< TType, TType2 > >( input );
        }
        else
        {
            m_slot.reset();
        }
    }

    template < typename TType >
    bool PushOutput< TType >::push( TType data )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        if( m_slot )
        {
            auto slot = m_slot;
            lock.unlock();
            return slot->push( std::forward< TType >( data ) );
        }
        else
        {
            return false;
        }
    }
}
