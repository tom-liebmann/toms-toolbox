#include <ttb/net/WriterThread.hpp>


namespace ttb
{
    WriterThread::WriterThread( Selector& selector )
        : m_selector( selector ), m_running( true ), m_thread( [this] { this->threadLoop(); } )
    {
    }

    WriterThread::~WriterThread()
    {
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            m_running = false;
            m_selector.interrupt();
        }

        m_thread.join();
    }

    void WriterThread::threadLoop()
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        while( m_running )
        {
            lock.unlock();

            m_selector.processWrites( true );

            lock.lock();
        }
    }
}
