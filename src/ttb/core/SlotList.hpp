#pragma once

#include <list>
#include <memory>

namespace core
{
    template < class T >
    class SlotList
    {
    public:
        class Slot
        {
        public:
            static void invalidate( Slot* slot );

            Slot( T instance );

            bool isValid() const;
            const T& getInstance() const;

        private:
            bool m_valid;
            T m_instance;
        };

        ~SlotList();

        std::shared_ptr< Slot > add( T instance );
        void traverse( const std::function< void( const T& ) >& callback ) const;

    private:
        mutable std::list< Slot* > m_slots;
    };
}



namespace ttb
{
    template < class T >
    SlotList< T >::~SlotList()
    {
        for( auto iter = m_slots.begin(); iter != m_slots.end(); iter = m_slots.erase( iter ) )
        {
            if( ( *iter )->isValid() )
                Slot::invalidate( *iter );
            else
                delete *iter;
        }
    }

    template < class T >
    std::shared_ptr< typename SlotList< T >::Slot > SlotList< T >::add( T instance )
    {
        auto slot = new Slot( std::move( instance ) );
        m_slots.push_back( slot );
        return std::shared_ptr< Slot >( slot, Slot::invalidate );
    }

    template < class T >
    void SlotList< T >::traverse( const std::function< void( const T& ) >& callback ) const
    {
        for( auto iter = m_slots.begin(); iter != m_slots.end(); )
        {
            if( ( *iter )->isValid() )
            {
                callback( ( *iter )->getInstance() );
                ++iter;
            }
            else
            {
                delete *iter;
                iter = m_slots.erase( iter );
            }
        }
    }



    template < class T >
    void SlotList< T >::Slot::invalidate( Slot* slot )
    {
        if( slot->isValid() )
            slot->m_valid = false;
        else
            delete slot;
    }

    template < class T >
    SlotList< T >::Slot::Slot( T instance ) : m_valid( true ), m_instance( std::move( instance ) )
    {
    }

    template < class T >
    inline bool SlotList< T >::Slot::isValid() const
    {
        return m_valid;
    }

    template < class T >
    inline const T& SlotList< T >::Slot::getInstance() const
    {
        return m_instance;
    }
}
