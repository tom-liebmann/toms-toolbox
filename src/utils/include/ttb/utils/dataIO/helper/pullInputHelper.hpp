#pragma once

#include <memory>


namespace ttb
{
    template < typename TType >
    class PullOutput;
}


namespace ttb
{
    namespace priv
    {
        template < typename TType >
        class PullInputSlot
        {
        public:
            virtual ~PullInputSlot();

            virtual TType pull() = 0;
        };


        template < typename TType, typename TType2 >
        class PullInputSlotImpl : public PullInputSlot< TType >
        {
        public:
            PullInputSlotImpl( std::shared_ptr< PullOutput< TType2 > > output );

            virtual TType pull() override;

        private:
            std::shared_ptr< PullOutput< TType2 > > m_output;
        };
    }
}


namespace ttb
{
    namespace priv
    {
        template < typename TType >
        PullInputSlot< TType >::~PullInputSlot()
        {
        }


        template < typename TType, typename TType2 >
        PullInputSlotImpl< TType, TType2 >::PullInputSlotImpl(
            std::shared_ptr< PullOutput< TType2 > > output )
            : m_output( std::move( output ) )
        {
        }

        template < typename TType, typename TType2 >
        TType PullInputSlotImpl< TType, TType2 >::pull()
        {
            return m_output->pull();
        }
    }
}
