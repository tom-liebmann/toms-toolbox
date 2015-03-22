#include "WindowMode.hpp"

#include <tgCore/Monitor.hpp>

using tg::WindowMode;

WindowMode::WindowMode( uint16_t width, uint16_t height, bool fullscreen )
    : m_width( width )
    , m_height( height )
    , m_fullscreen( fullscreen )
    , m_monitor( Monitor::fromPrimary() )
{ }
