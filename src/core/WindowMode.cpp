#include "WindowMode.hpp"

#include <core/Monitor.hpp>

tg::WindowMode::WindowMode( uint16_t width, uint16_t height, bool fullscreen )
    : m_width( width )
    , m_height( height )
    , m_fullscreen( fullscreen )
    , m_monitor( Monitor::fromPrimary() )
{ }
