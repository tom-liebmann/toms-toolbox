#include "Monitor.hpp"

using tg::Monitor;

std::shared_ptr< Monitor > Monitor::fromPrimary()
{
    return std::shared_ptr< Monitor >( new Monitor( glfwGetPrimaryMonitor() ) );
}

Monitor::Monitor( GLFWmonitor* monitor )
    : m_monitor( monitor )
{ }
