#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <memory>

// declarations
//=============================================================================

namespace ttb
{
    class Monitor
    {
    public:
        static std::shared_ptr< Monitor > fromPrimary();

        GLFWmonitor* getHandle() const;

    private:
        Monitor( GLFWmonitor* monitor );

        GLFWmonitor* m_handle;
    };
}



// definitions
//=============================================================================

namespace ttb
{
    inline GLFWmonitor* Monitor::getHandle() const
    {
        return m_handle;
    }
}
