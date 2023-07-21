#include <ttb/project/Application.hpp>

#include <ttb/project/AdManager.hpp>


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
