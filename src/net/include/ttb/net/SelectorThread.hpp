#pragma once

#include <ttb/net/NetSelector.hpp>

#include <mutex>
#include <thread>


namespace ttb
{
    class SelectorThread
    {
    public:
        SelectorThread( Selector& selector );

        ~SelectorThread();

    private:
        void threadLoop();

        Selector& m_selector;

        bool m_running;
        std::mutex m_mutex;
        std::thread m_thread;
    };
}
