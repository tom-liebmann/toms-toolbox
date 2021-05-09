#include <ttb/project/Application.hpp>


namespace ttb
{
    Application::~Application() = default;

    void Application::stop()
    {
        m_stopped = true;
    }

    bool Application::stopped() const
    {
        return m_stopped;
    }
}
