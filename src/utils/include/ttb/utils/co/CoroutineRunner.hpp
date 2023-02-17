#pragma once

#include <ttb/utils/co/Coroutine.hpp>
#include <ttb/utils/co/helper.hpp>

#include <fmt/core.h>

#include <list>
#include <memory>
#include <type_traits>


namespace ttb::co
{
    class CoroutineRunner
    {
    public:
        template < typename TType >
        using Callback = helper::CallbackType< TType >::type;

        using ExceptionCallback = std::function< void( std::exception_ptr ) >;

        void run();

        template < typename TType >
        void push( Coroutine< TType > coroutine,
                   Callback< TType > callback,
                   ExceptionCallback exceptionCallback );

    private:
        class Slot;

        template < typename TType >
        class TypedSlot;

        std::list< std::unique_ptr< Slot > > m_slots;
    };


    class CoroutineRunner::Slot
    {
    public:
        virtual ~Slot();

        /**
         * @returns True if coroutine is still running
         */
        virtual bool run() = 0;
    };


    template < typename TType >
    class CoroutineRunner::TypedSlot : public Slot
    {
    public:
        TypedSlot( Coroutine< TType >&& coroutine,
                   Callback< TType >&& callback,
                   ExceptionCallback&& exceptionCallback );

        virtual bool run() override;

    private:
        Coroutine< TType > m_coroutine;
        Callback< TType > m_callback;
        ExceptionCallback m_exceptionCallback;
    };
}


namespace ttb::co
{
    template < typename TType >
    void CoroutineRunner::push( Coroutine< TType > coroutine,
                                Callback< TType > callback,
                                ExceptionCallback exceptionCallback )
    {
        auto slot = std::make_unique< TypedSlot< TType > >(
            std::move( coroutine ), std::move( callback ), std::move( exceptionCallback ) );

        if( slot->run() )
        {
            m_slots.push_back( std::move( slot ) );
        }
    }


    template < typename TType >
    CoroutineRunner::TypedSlot< TType >::TypedSlot( Coroutine< TType >&& coroutine,
                                                    Callback< TType >&& callback,
                                                    ExceptionCallback&& exceptionCallback )
        : m_coroutine{ std::move( coroutine ) }
        , m_callback{ std::move( callback ) }
        , m_exceptionCallback{ std::move( exceptionCallback ) }
    {
    }

    template < typename TType >
    bool CoroutineRunner::TypedSlot< TType >::run()
    {
        if( m_coroutine.resume() )
        {
            return true;
        }

        if( auto exception = m_coroutine.getException() )
        {
            m_exceptionCallback( exception );
            return false;
        }

        if constexpr( std::is_void_v< TType > )
        {
            m_callback();
        }
        else
        {
            m_callback( std::move( m_coroutine.getResult().value() ) );
        }

        return false;
    }
}
