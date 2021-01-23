#pragma once

#include <list>


namespace ttb
{
    template < typename TIndex >
    class IndexManager
    {
    public:
        TIndex next();

        void free( TIndex index );

    private:
        std::list< TIndex > m_freeIndices;
        TIndex m_nextIndex{};
    };
}


namespace ttb
{
    template < typename TIndex >
    TIndex IndexManager< TIndex >::next()
    {
        if( m_freeIndices.empty() )
        {
            return m_nextIndex++;
        }
        else
        {
            auto const index = m_freeIndices.front();
            m_freeIndices.pop_front();
            return index;
        }
    }

    template < typename TIndex >
    void IndexManager< TIndex >::free( TIndex index )
    {
        m_freeIndices.push_back( index );
    }
}
