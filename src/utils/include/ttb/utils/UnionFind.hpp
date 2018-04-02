#pragma once

#include <numeric>


namespace ttb
{
    class UnionFind
    {
    public:
        UnionFind( size_t size );

        size_t merge( size_t lhs, size_t rhs );

        size_t find( size_t lhs ) const;

    private:
        std::vector< size_t > m_parent;
    };
}


namespace ttb
{
    UnionFind::UnionFind( size_t size ) : m_parent( size )
    {
        std::iota( std::begin( m_parent ), std::end( m_parent ), 0 );
    }

    size_t UnionFind::merge( size_t lhs, size_t rhs )
    {
        auto pl = find( lhs );
        auto pr = find( rhs );

        m_parent[ pl ] = pr;

        return pr;
    }

    size_t UnionFind::find( size_t lhs ) const
    {
        auto p = m_parent[ lhs ];
        return p == lhs ? p : find( p );
    }
}
