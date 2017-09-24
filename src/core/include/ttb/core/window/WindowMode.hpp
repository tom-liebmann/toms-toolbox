#pragma once

#include <cstdint>
#include <memory>


// declarations
//=============================================================================

namespace ttb
{
    class WindowMode
    {
    public:
        WindowMode( uint16_t width, uint16_t height, bool fullscreen );

        uint16_t width() const;

        uint16_t height() const;

        bool fullscreen() const;

    private:
        uint16_t m_width;
        uint16_t m_height;
        bool m_fullscreen;
    };
}


// definitions
//=============================================================================

namespace ttb
{
    inline WindowMode::WindowMode( uint16_t width, uint16_t height, bool fullscreen )
        : m_width( width ), m_height( height ), m_fullscreen( fullscreen )
    {
    }

    inline uint16_t WindowMode::width() const
    {
        return m_width;
    }

    inline uint16_t WindowMode::height() const
    {
        return m_height;
    }

    inline bool WindowMode::fullscreen() const
    {
        return m_fullscreen;
    }
}
