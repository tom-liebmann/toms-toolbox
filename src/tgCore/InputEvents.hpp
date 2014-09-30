#pragma once

#include <tgCore/Window.hpp>

namespace tgCore
{
    class KeyEvent
        : public Window::Event
    {
        public:
            enum class Action
            {
                DOWN,
                UP
            };

            KeyEvent( uint32_t key, Action action );

            uint32_t getKey() const;
            Action getAction() const;

        private:
            uint32_t m_key;
            Action m_keyAction;
    };

    class MouseButtonEvent
        : public Window::Event
    {
        public:
            enum class Button
            {
                LEFT,
                RIGHT,
                MIDDLE
            };

            enum class Action
            {
                DOWN,
                UP
            };

            MouseButtonEvent(
                Button button,
                Action action,
                double x,
                double y );

            Button getButton() const;
            Action getAction() const;
            double getX() const;
            double getY() const;

        private:
            Button m_button;
            Action m_action;
            double m_x;
            double m_y;

    };



    inline KeyEvent::KeyEvent( Window::Event::Type type, uint32_t key, KeyAction action )
        : Window::Event( Window::Event::Type::KEY )
        , m_type( type )
        , m_key( key )
        , m_action( action )
    { }

    inline uint32_t KeyEvent::getKey() const
    {
        return m_key;
    }

    inline uint32_t KeyEvent::getKeyAction() const
    {
        return m_keyAction;
    }
}
