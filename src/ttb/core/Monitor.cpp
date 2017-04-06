#include "Monitor.hpp"

std::shared_ptr< ttb::Monitor > ttb::Monitor::fromPrimary()
{
    return std::shared_ptr< Monitor >( new Monitor( glfwGetPrimaryMonitor() ) );
}

ttb::Monitor::Monitor( GLFWmonitor* handle ) : m_handle( handle )
{
}
