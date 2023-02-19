#include <ttb/utils/co/CoroutineRunner.hpp>


namespace ttb::co
{
    void CoroutineRunner::run()
    {
        for( auto iter = std::begin( m_slots ); iter != std::end( m_slots ); )
        {
            if( !( *iter )->run() )
            {
                iter = m_slots.erase( iter );
            }
            else
            {
                ++iter;
            }
        }
    }

    std::size_t CoroutineRunner::getCoroutineCount() const
    {
        return m_slots.size();
    }


    CoroutineRunner::Slot::~Slot() = default;
}
