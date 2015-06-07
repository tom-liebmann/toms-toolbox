#include "Monitor.hpp"

std::shared_ptr< tg::Monitor > tg::Monitor::fromPrimary()
{
    return std::shared_ptr< Monitor >( new Monitor( glfwGetPrimaryMonitor() ) );
}

tg::Monitor::Monitor( GLFWmonitor* handle )
    : m_handle( handle )
{ }
