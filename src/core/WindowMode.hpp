#pragma once

#include <cstdint>
#include <memory>

// forward declarations
//=============================================================================

namespace tg
{
    class Monitor;
}



// declarations
//=============================================================================

namespace tg
{
    class WindowMode
    {
    public:
        WindowMode( uint16_t width, uint16_t height, bool fullscreen );

        uint16_t getWidth() const;
        uint16_t getHeight() const;
        bool isFullscreen() const;
        const Monitor& getMonitor() const;

    private:
        uint16_t m_width;
        uint16_t m_height;
        bool m_fullscreen;
        std::shared_ptr< Monitor > m_monitor;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline uint16_t WindowMode::getWidth() const
    {
        return m_width;
    }

    inline uint16_t WindowMode::getHeight() const
    {
        return m_height;
    }

    inline bool WindowMode::isFullscreen() const
    {
        return m_fullscreen;
    }

    inline const Monitor& WindowMode::getMonitor() const
    {
        return *m_monitor;
    }
}
