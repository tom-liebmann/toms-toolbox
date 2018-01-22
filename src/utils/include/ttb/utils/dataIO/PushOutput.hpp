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
        void input( std::shared_ptr< PushInput< TType2 > > input );

        void push( TType data );

    private:
        std::mutex m_mutex;
        std::shared_ptr< priv::PushOutputSlot< TType > > m_slot;
    };
}


namespace ttb
{
    template < typename TType >
    template < typename TType2 >
    void PushOutput< TType >::input( std::shared_ptr< PushInput< TType2 > > input )
    {
        std::lock_guard< std::mutex > lock( m_mutex );

        m_slot =
            std::make_shared< priv::PushOutputSlotImpl< TType, TType2 > >( std::move( input ) );
    }

    template < typename TType >
    void PushOutput< TType >::push( TType data )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        if( m_slot )
        {
            auto slot = m_slot;

            lock.unlock();

            slot->push( std::forward< TType >( data ) );
        }
    }
}
