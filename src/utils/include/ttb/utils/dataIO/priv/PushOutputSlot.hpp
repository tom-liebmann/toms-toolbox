#pragma once

#include <memory>
#include <type_traits>


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

            virtual bool push( TType data ) = 0;
        };


        template < typename TType, typename TType2 >
        class PushOutputSlotImpl : public PushOutputSlot< TType >
        {
        public:
            PushOutputSlotImpl( std::shared_ptr< PushInputSlot< TType2 > > const& inputSlot );

            virtual bool push( TType data ) override;

        private:
            std::shared_ptr< PushInputSlot< TType2 > > m_inputSlot;
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
            std::shared_ptr< PushInputSlot< TType2 > > const& inputSlot )
            : m_inputSlot( inputSlot )
        {
        }

        template < typename TType, typename TType2 >
        bool PushOutputSlotImpl< TType, TType2 >::push( TType data )
        {
            if( m_inputSlot )
            {
                m_inputSlot->push( std::forward< TType >( data ) );
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
