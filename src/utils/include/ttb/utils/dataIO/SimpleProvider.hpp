#pragma once

#include <ttb/utils/dataIO/Provider.hpp>
#include <ttb/utils/dataIO/Receiver.hpp>

#include <condition_variable>
#include <functional>
#include <mutex>


namespace ttb
{
    template < SlotType TType, typename TData >
    class SimpleProvider;
}


namespace ttb
{
    template < typename TData >
    class SimpleProvider< SlotType::ACTIVE, TData > : public Provider< SlotType::ACTIVE, TData >
    {
    public:
        void push( TData data );

        // Override: Provider
        virtual void
            receiver( std::shared_ptr< Receiver< SlotType::PASSIVE, TData > > receiver ) override;

    private:
        std::shared_ptr< Receiver< SlotType::PASSIVE, TData > > m_receiver;
        std::mutex m_mutex;
        std::condition_variable m_condition;
    };


    template < typename TData >
    class SimpleProvider< SlotType::PASSIVE, TData > : public Provider< SlotType::PASSIVE, TData >
    {
    public:
        using Callback = std::function< TData() >;

        SimpleProvider( Callback callback );

        // Override: Provider
        virtual TData pull() override;

    private:
        Callback m_callback;
    };
}


namespace ttb
{
    template < typename TData >
    void SimpleProvider< SlotType::ACTIVE, TData >::push( TData data )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        if( m_receiver )
        {
            auto receiver = m_receiver;

            lock.unlock();

            receiver->push( std::forward< TData >( data ) );
        }
    }

    template < typename TData >
    void SimpleProvider< SlotType::ACTIVE, TData >::receiver(
        std::shared_ptr< Receiver< SlotType::PASSIVE, TData > > receiver )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        m_receiver = std::move( receiver );

        if( m_receiver )
        {
            lock.unlock();

            m_condition.notify_one();
        }
    }


    template < typename TData >
    SimpleProvider< SlotType::PASSIVE, TData >::SimpleProvider( Callback callback )
        : m_callback( callback )
    {
    }

    template < typename TData >
    TData SimpleProvider< SlotType::PASSIVE, TData >::pull()
    {
        if( m_callback )
        {
            return m_callback();
        }
        {
            throw std::runtime_error( "No callback set" );
        }
    }
}
