//#include "EventSource.hpp"
//
//namespace ttb
//{
//    EventSource::~EventSource() = default;
//
//    void EventSource::addListener( std::shared_ptr< EventListener > const& listener )
//    {
//        m_listener.push_back( listener );
//    }
//
//    void EventSource::removeListener( std::shared_ptr< EventListener > const& listener )
//    {
//        auto iter = std::find( std::begin( m_listener ), std::end( m_listener ) );
//        if( iter != std::end( m_listener ) )
//            m_listener.erase( iter );
//    }
//
//    void EventSource::handleEvent( Event const& event ) const
//    {
//        for( auto const& listener : m_listener )
//            listener->handleEvent( event );
//    }
//}
