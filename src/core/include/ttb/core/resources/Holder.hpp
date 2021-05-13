#pragma once

#include <memory>


namespace ttb::resources
{
    template < typename TType, typename TCreator >
    class Holder
    {
    public:
        template < typename TType2 >
        Holder( TType2 creator );

        void acquire();

        void release();

        TType& get();

        TType const& get() const;

    private:
        TCreator m_creator;
        uint32_t m_useCount{ 0 };
        std::unique_ptr< TType > m_resource;
    };


    template < typename TType2 >
    Holder( TType2 creator )
        -> Holder< typename std::result_of_t< TType2() >::element_type, TType2 >;


    template < typename TType, typename TCreator >
    template < typename TType2 >
    Holder< TType, TCreator >::Holder( TType2 creator ) : m_creator{ std::move( creator ) }
    {
    }

    template < typename TType, typename TCreator >
    void Holder< TType, TCreator >::acquire()
    {
        if( 0 == m_useCount )
        {
            m_resource = m_creator();
        }

        ++m_useCount;
    }

    template < typename TType, typename TCreator >
    void Holder< TType, TCreator >::release()
    {
        --m_useCount;

        if( 0 == m_useCount )
        {
            m_resource.reset();
        }
    }

    template < typename TType, typename TCreator >
    TType& Holder< TType, TCreator >::get()
    {
        return *m_resource;
    }

    template < typename TType, typename TCreator >
    TType const& Holder< TType, TCreator >::get() const
    {
        return *m_resource;
    }
}
