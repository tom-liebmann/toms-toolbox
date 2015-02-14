#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

namespace tgCore
{
    class Monitor
    {
        public:
            static std::shared_ptr< Monitor > fromPrimary();

            GLFWmonitor* getHandle() const;

        private:
            Monitor( GLFWmonitor* monitor );

            GLFWmonitor* m_monitor;
    };



    inline GLFWmonitor* Monitor::getHandle() const
    {
        return m_monitor;
    }
}
