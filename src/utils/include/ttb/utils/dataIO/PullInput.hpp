#pragma once

#include "helper/pullInputHelper.hpp"

#include <mutex>


namespace ttb
{
    template < typename TType >
    class PullInput
    {
    public:
        template < typename TType2 >
        void output( std::shared_ptr< PullOutput< TType2 > > output );

        TType pull();

    private:
        std::mutex m_mutex;
        std::shared_ptr< priv::PullInputSlot< TType > > m_slot;
    };
}


namespace ttb
{
    template < typename TType >
    template < typename TType2 >
    void PullInput< TType >::output( std::shared_ptr< PullOutput< TType2 > > output )
    {
        std::lock_guard< std::mutex > lock( m_mutex );

        if( output )
            m_slot =
                std::make_shared< priv::PullInputSlotImpl< TType, TType2 > >( std::move( output ) );
        else
            m_slot.reset();
    }

    template < typename TType >
    TType PullInput< TType >::pull()
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        if( !m_slot )
            throw std::runtime_error( "Pull from unconnected input" );

        auto slot = m_slot;

        lock.unlock();

        return slot->pull();
    }
}
