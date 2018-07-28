#pragma once

#include <ttb/net/NetSelector.hpp>

#include <mutex>
#include <thread>


namespace ttb
{
    class SelectorThread
    {
    public:
        SelectorThread( NetSelector& selector );

        ~SelectorThread();

    private:
        void threadLoop();

        NetSelector& m_selector;

        bool m_running;
        std::mutex m_mutex;
        std::thread m_thread;
    };
}
