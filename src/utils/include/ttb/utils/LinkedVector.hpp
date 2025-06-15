#pragma once

#include <cstdlib>
#include <vector>


namespace ttb
{
    template < typename TType >
    class LinkedVector
    {
    public:
        class Iterator;
        class ConstIterator;

        LinkedVector();

        size_t add( TType const& value );

        size_t add( TType&& value );

        TType& get( size_t index );

        TType const& get( size_t index ) const;

        void remove( size_t index );

        size_t size() const;

        Iterator begin();

        Iterator end();

        ConstIterator begin() const;

        ConstIterator end() const;

    private:
        struct Container
        {
            bool used;
            size_t nextUnused;
            TType value;
        };

        size_t m_size;
        size_t m_firstUnused;
        std::vector< Container > m_values;

        friend class Iterator;
        friend class ConstIterator;
    };


    template < typename TType >
    class LinkedVector< TType >::Iterator
    {
    public:
        using difference_type = ssize_t;
        using value_type = TType;
        using pointer = TType*;
        using reference = TType&;
        using iterator_category = std::forward_iterator_tag;

        Iterator( LinkedVector& vector, size_t index );

        reference operator*();

        Iterator& operator++();

        bool operator!=( Iterator const& rhs ) const;

        size_t index() const;

    private:
        LinkedVector& m_vector;
        size_t m_index;
    };


    template < typename TType >
    class LinkedVector< TType >::ConstIterator
    {
    public:
        using difference_type = ssize_t;
        using value_type = TType const;
        using pointer = TType const*;
        using reference = TType const&;
        using iterator_category = std::forward_iterator_tag;

        ConstIterator( LinkedVector const& vector, size_t index );

        reference operator*();

        ConstIterator& operator++();

        bool operator!=( ConstIterator const& rhs ) const;

        size_t index() const;

    private:
        LinkedVector const& m_vector;
        size_t m_index;
    };
}


namespace ttb
{
    template < typename TType >
    LinkedVector< TType >::LinkedVector() : m_size{ 0 }, m_firstUnused{ 0 }
    {
    }

    template < typename TType >
    size_t LinkedVector< TType >::add( TType const& value )
    {
        if( m_size < m_values.size() )
        {
            size_t const slotId = m_firstUnused;
            auto& slot = m_values[ slotId ];
            m_firstUnused = slot.nextUnused;
            slot.used = true;
            slot.value = value;
            ++m_size;
            return slotId;
        }
        else
        {
            m_values.push_back( Container{ true, 0, value } );
            ++m_size;
            return m_values.size() - 1;
        }
    }

    template < typename TType >
    size_t LinkedVector< TType >::add( TType&& value )
    {
        if( m_size < m_values.size() )
        {
            size_t const slotId = m_firstUnused;
            auto& slot = m_values[ slotId ];
            m_firstUnused = slot.nextUnused;
            slot.used = true;
            slot.value = std::move( value );
            ++m_size;
            return slotId;
        }
        else
        {
            m_values.push_back( Container{ true, 0, std::move( value ) } );
            ++m_size;
            return m_values.size() - 1;
        }
    }

    template < typename TType >
    TType& LinkedVector< TType >::get( size_t index )
    {
        return m_values[ index ].value;
    }

    template < typename TType >
    TType const& LinkedVector< TType >::get( size_t index ) const
    {
        return m_values[ index ].value;
    }

    template < typename TType >
    void LinkedVector< TType >::remove( size_t index )
    {
        --m_size;
        m_values[ index ].used = false;
        m_values[ index ].nextUnused = m_firstUnused;
        m_firstUnused = index;
    }

    template < typename TType >
    size_t LinkedVector< TType >::size() const
    {
        return m_size;
    }

    template < typename TType >
    typename LinkedVector< TType >::Iterator LinkedVector< TType >::begin()
    {
        return { *this, 0 };
    }

    template < typename TType >
    typename LinkedVector< TType >::Iterator LinkedVector< TType >::end()
    {
        return { *this, m_values.size() };
    }

    template < typename TType >
    typename LinkedVector< TType >::ConstIterator LinkedVector< TType >::begin() const
    {
        return { *this, 0 };
    }

    template < typename TType >
    typename LinkedVector< TType >::ConstIterator LinkedVector< TType >::end() const
    {
        return { *this, m_values.size() };
    }


    template < typename TType >
    LinkedVector< TType >::Iterator::Iterator( LinkedVector& vector, size_t index )
        : m_vector{ vector }, m_index{ index }
    {
        while( m_index < m_vector.m_values.size() && !m_vector.m_values[ m_index ].used )
        {
            ++m_index;
        }
    }

    template < typename TType >
    TType& LinkedVector< TType >::Iterator::operator*()
    {
        return m_vector.m_values[ m_index ].value;
    }

    template < typename TType >
    typename LinkedVector< TType >::Iterator& LinkedVector< TType >::Iterator::operator++()
    {
        ++m_index;
        while( m_index < m_vector.m_values.size() && !m_vector.m_values[ m_index ].used )
        {
            ++m_index;
        }
        return *this;
    }

    template < typename TType >
    bool LinkedVector< TType >::Iterator::operator!=( Iterator const& rhs ) const
    {
        return m_index != rhs.m_index;
    }

    template < typename TType >
    size_t LinkedVector< TType >::Iterator::index() const
    {
        return m_index;
    }


    template < typename TType >
    LinkedVector< TType >::ConstIterator::ConstIterator( LinkedVector const& vector, size_t index )
        : m_vector( vector ), m_index( index )
    {
        while( m_index < m_vector.m_values.size() && !m_vector.m_values[ m_index ].used )
        {
            ++m_index;
        }
    }

    template < typename TType >
    TType const& LinkedVector< TType >::ConstIterator::operator*()
    {
        return m_vector.m_values[ m_index ].value;
    }

    template < typename TType >
    typename LinkedVector< TType >::ConstIterator&
        LinkedVector< TType >::ConstIterator::operator++()
    {
        ++m_index;
        while( m_index < m_vector.m_values.size() && !m_vector.m_values[ m_index ].used )
        {
            ++m_index;
        }
        return *this;
    }

    template < typename TType >
    bool LinkedVector< TType >::ConstIterator::operator!=( ConstIterator const& rhs ) const
    {
        return m_index != rhs.m_index;
    }

    template < typename TType >
    size_t LinkedVector< TType >::ConstIterator::index() const
    {
        return m_index;
    }
}
