#pragma once

#include <ttb/utils/dataIO/Receiver.hpp>

#include <condition_variable>
#include <memory>
#include <mutex>


namespace ttb
{
    template < SlotType TType, typename TData >
    class SimpleReceiver;
}


namespace ttb
{
    template < typename TData >
    class SimpleReceiver< SlotType::ACTIVE, TData > : public Receiver< SlotType::ACTIVE, TData >
    {
    public:
        TData pull();

        // Override: Receiver
        virtual void
            provider( std::shared_ptr< Provider< SlotType::PASSIVE, TData > > provider ) override;

    private:
        std::shared_ptr< Provider< SlotType::PASSIVE, TData > > m_provider;
        std::mutex m_mutex;
        std::condition_variable m_condition;
    };


    template < typename TData >
    class SimpleReceiver< SlotType::PASSIVE, TData > : public Receiver< SlotType::PASSIVE, TData >
    {
    public:
        using Callback = std::function< void( TData&& ) >;

        SimpleReceiver( Callback callback );

        // Override: Receiver
        virtual void push( TData data ) override;

    private:
        Callback m_callback;
    };
}


namespace ttb
{
    template < typename TData >
    TData SimpleReceiver< SlotType::ACTIVE, TData >::pull()
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        while( !m_provider )
        {
            m_condition.wait( lock );
        }

        auto provider = m_provider;

        lock.unlock();

        return provider->pull();
    }

    template < typename TData >
    void SimpleReceiver< SlotType::ACTIVE, TData >::provider(
        std::shared_ptr< Provider< SlotType::PASSIVE, TData > > provider )
    {
        std::unique_lock< std::mutex > lock( m_mutex );
        m_provider = std::move( provider );

        if( m_provider )
        {
            lock.unlock();

            m_condition.notify_one();
        }
    }


    template < typename TData >
    SimpleReceiver< SlotType::PASSIVE, TData >::SimpleReceiver( Callback callback )
        : m_callback( callback )
    {
    }

    template < typename TData >
    void SimpleReceiver< SlotType::PASSIVE, TData >::push( TData data )
    {
        if( m_callback )
        {
            m_callback( std::move( data ) );
        }
        else
        {
            throw std::runtime_error( "Callback not set" );
        }
    }
}
