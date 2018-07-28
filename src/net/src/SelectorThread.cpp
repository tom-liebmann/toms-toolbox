#include <ttb/net/SelectorThread.hpp>


namespace ttb
{
    SelectorThread::SelectorThread( NetSelector& selector )
        : m_selector( selector ), m_running( true ), m_thread( [this] { this->threadLoop(); } )
    {
    }

    SelectorThread::~SelectorThread()
    {
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            m_running = false;
            m_selector.interrupt();
        }

        m_thread.join();
    }

    void SelectorThread::threadLoop()
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        while( m_running )
        {
            lock.unlock();

            m_selector.process( true );

            lock.lock();
        }
    }
}
