#pragma once

#include "WindowFlag.hpp"


namespace ttb
{
    class WindowMode
    {
    public:
        WindowMode( uint16_t width, uint16_t height, WindowFlag flags = WindowFlag::NONE );

        uint16_t width() const;

        uint16_t height() const;

        bool flag( WindowFlag value ) const;

        WindowFlag flags() const;

    private:
        uint16_t m_width;
        uint16_t m_height;
        WindowFlag m_flags;
    };
}


namespace ttb
{
    inline WindowMode::WindowMode( uint16_t width, uint16_t height, WindowFlag flags )
        : m_width{ width }, m_height{ height }, m_flags{ flags }
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

    inline bool WindowMode::flag( WindowFlag value ) const
    {
        return static_cast< uint32_t >( m_flags ) & static_cast< uint32_t >( value );
    }

    inline WindowFlag WindowMode::flags() const
    {
        return m_flags;
    }
}
