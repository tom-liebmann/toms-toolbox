#pragma once

#include <boost/asio.hpp>

#include <memory>
#include <thread>


namespace ttb::net
{
    class ContextThread
    {
    public:
        ContextThread();

        ~ContextThread();

        std::shared_ptr< boost::asio::io_context > const& context();

        void notify();

    private:
        void run();

        std::shared_ptr< boost::asio::io_context > m_context;

        bool m_running;
        bool m_workAvailable;
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::thread m_thread;
    };
}
