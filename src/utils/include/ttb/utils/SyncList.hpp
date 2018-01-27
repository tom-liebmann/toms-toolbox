#pragma once

#include <algorithm>
#include <mutex>
#include <tuple>
#include <vector>



// declarations
//=============================================================================

namespace ttb
{
    template < typename T >
    class SyncList
    {
    public:
        void push_back( T value );

        void erase( T value );

        template < typename Op >
        void iterate( Op op );

    private:
        std::mutex m_mutex;
        size_t m_iterationCount;
        std::vector< std::tuple< uint8_t, T > > m_changes;
        std::vector< T > m_values;
    };
}



// definitions
//=============================================================================

namespace ttb
{
    template < typename T >
    void SyncList< T >::push_back( T value )
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_changes.emplace_back( 0, std::move( value ) );
    }

    template < typename T >
    void SyncList< T >::erase( T value )
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_changes.emplace_back( 1, std::move( value ) );
    }

    template < typename T >
    template < typename Op >
    void SyncList< T >::iterate( Op op )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        if( m_iterationCount == 0 )
        {
            for( auto& change : m_changes )
            {
                switch( std::get< 0 >( change ) )
                {
                    case 0:  // Insertion
                        m_values.push_back( std::move( std::get< 1 >( change ) ) );
                        break;

                    case 1:  // Deletion
                        m_values.erase( std::find(
                            m_values.begin(), m_values.end(), std::get< 1 >( change ) ) );
                        break;
                }
            }

            m_changes.clear();
        }

        ++m_iterationCount;

        lock.unlock();

        for( auto& value : m_values )
        {
            op( value );
        }

        lock.lock();

        --m_iterationCount;
    }
}
