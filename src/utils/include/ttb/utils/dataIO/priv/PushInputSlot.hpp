#pragma once

#include <mutex>


namespace ttb
{
    template < typename TType >
    class PushInput;
}


namespace ttb
{
    namespace priv
    {
        template < typename TType >
        class PushInputSlot
        {
        public:
            PushInputSlot( PushInput< TType >& input );

            template < typename TType2 >
            bool push( TType2&& data );

        private:
            // Called by the PushInput before it gets destroyed.
            // Locks the PushInput to ensure multi-threaded access.
            void disable();

            std::mutex m_mutex;

            PushInput< TType >* m_input;

            friend class PushInput< TType >;
        };
    }
}


namespace ttb
{
    namespace priv
    {
        template < typename TType >
        PushInputSlot< TType >::PushInputSlot( PushInput< TType >& input ) : m_input( &input )
        {
        }

        template < typename TType >
        template < typename TType2 >
        bool PushInputSlot< TType >::push( TType2&& data )
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            if( m_input )
            {
                m_input->push( std::forward< TType2 >( data ) );
                return true;
            }
            else
            {
                return false;
            }
        }

        template < typename TType >
        void PushInputSlot< TType >::disable()
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            m_input = nullptr;
        }
    }
}
