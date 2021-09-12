#pragma once

#include <cstdlib>
#include <numeric>
#include <vector>


namespace ttb
{
    class UnionFind
    {
    public:
        UnionFind();

        UnionFind( size_t size );

        size_t size() const;

        size_t merge( size_t lhs, size_t rhs );

        size_t find( size_t lhs ) const;

        void resize( size_t size );

        void reset();

    private:
        std::vector< size_t > m_parent;
    };
}


namespace ttb
{
    inline UnionFind::UnionFind()
    {
    }

    inline UnionFind::UnionFind( size_t size ) : m_parent( size )
    {
        reset();
    }

    inline size_t UnionFind::size() const
    {
        return m_parent.size();
    }

    inline size_t UnionFind::merge( size_t lhs, size_t rhs )
    {
        auto pl = find( lhs );
        auto pr = find( rhs );

        m_parent[ pl ] = pr;

        return pr;
    }

    inline size_t UnionFind::find( size_t lhs ) const
    {
        auto p = m_parent[ lhs ];
        return p == lhs ? p : find( p );
    }

    inline void UnionFind::resize( size_t size )
    {
        m_parent.resize( size );
        reset();
    }

    inline void UnionFind::reset()
    {
        std::iota( std::begin( m_parent ), std::end( m_parent ), 0 );
    }
}
