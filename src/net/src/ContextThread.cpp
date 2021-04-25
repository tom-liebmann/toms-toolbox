#include "ContextThread.hpp"

#include <iostream>


namespace ttb::net
{
    ContextThread::ContextThread()
        : m_context( std::make_shared< boost::asio::io_context >() )
        , m_running( true )
        , m_workAvailable( true )
        , m_thread( [ this ] { run(); } )
    {
    }

    ContextThread::~ContextThread()
    {
        // TODO figure out if this is safe in all circumstances. It could be that the context
        // thread is owned by a job of the m_context, in which case this destructor would get
        // called from inside the run method. I currently just can't wrap my head around that.

        {
            std::lock_guard< std::mutex > lock( m_mutex );
            m_running = false;
            m_context->stop();
            m_condition.notify_one();
        }

        m_thread.join();
    }

    std::shared_ptr< boost::asio::io_context > const& ContextThread::context()
    {
        return m_context;
    }

    void ContextThread::notify()
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_workAvailable = true;
        m_condition.notify_one();
    }

    void ContextThread::run()
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        while( m_running )
        {
            if( m_workAvailable )
            {
                if( m_context->stopped() )
                {
                    m_context->restart();
                }

                m_workAvailable = false;

                lock.unlock();

                m_context->run();

                lock.lock();
            }
            else
            {
                m_condition.wait( lock );
            }
        }
    }
}
