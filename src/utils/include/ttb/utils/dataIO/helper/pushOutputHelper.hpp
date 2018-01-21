#pragma once

#include <memory>


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
        class PushOutputSlot
        {
        public:
            virtual ~PushOutputSlot();

            virtual void push( TType const& data ) = 0;
        };


        template < typename TType, typename TType2 >
        class PushOutputSlotImpl : public PushOutputSlot< TType >
        {
        public:
            PushOutputSlotImpl( std::shared_ptr< PushInput< TType2 > > input );

            virtual void push( TType const& data ) override;

        private:
            std::shared_ptr< PushInput< TType2 > > m_input;
        };
    }
}


namespace ttb
{
    namespace priv
    {
        template < typename TType >
        PushOutputSlot< TType >::~PushOutputSlot()
        {
        }


        template < typename TType, typename TType2 >
        PushOutputSlotImpl< TType, TType2 >::PushOutputSlotImpl(
            std::shared_ptr< PushInput< TType2 > > input )
            : m_input( std::move( input ) )
        {
        }

        template < typename TType, typename TType2 >
        void PushOutputSlotImpl< TType, TType2 >::push( TType const& data )
        {
            m_input->push( data );
        }
    }
}
