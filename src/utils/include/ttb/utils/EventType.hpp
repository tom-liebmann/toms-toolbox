#pragma once

#include <cstdint>


namespace ttb::event::type
{
    constexpr uint32_t KEY = 0x0;
    constexpr uint32_t WINDOW_CLOSE = 0x1;
    constexpr uint32_t WINDOW_RESIZE = 0x2;
    constexpr uint32_t SCROLL = 0x3;

    constexpr uint32_t POINTER_DOWN = 0x4;
    constexpr uint32_t POINTER_UP = 0x5;
    constexpr uint32_t POINTER_MOVE = 0x6;
    constexpr uint32_t POINTER_PRESS_START = 0x7;
    constexpr uint32_t POINTER_PRESS_ABORT = 0x8;
    constexpr uint32_t POINTER_PRESS_END = 0x9;

    constexpr uint32_t DRAG_START = 0xA;
    constexpr uint32_t DRAG_MOVE = 0xB;
    constexpr uint32_t DRAG_END = 0xC;

    constexpr uint32_t ZOOM = 0xD;
}
