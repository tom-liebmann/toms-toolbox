#pragma once

#include <ttb/net/NetSelector.hpp>

#include <mutex>
#include <thread>


namespace ttb
{
    class WriterThread
    {
    public:
        WriterThread( Selector& selector );

        ~WriterThread();

    private:
        void threadLoop();

        Selector& m_selector;

        bool m_running;
        std::mutex m_mutex;
        std::thread m_thread;
    };
}
