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

            KeyEvent( uint32_t key, Action action )
                : Window::Event( Window::Event::Type::KEY )
                , m_key( key )
                , m_action( action )
            { }

            uint32_t getKey() const
            {
                return m_key;
            }

            Action getAction() const
            {
                return m_action;
            }

        private:
            uint32_t m_key;
            Action m_action;
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
                double y )
                : Window::Event( Window::Event::Type::MOUSE_BUTTON )
                , m_button( button )
                , m_action( action )
                , m_x( x )
                , m_y( y )
            { }

            Button getButton() const
            {
                return m_button;
            }

            Action getAction() const
            {
                return m_action;
            }

            double getX() const
            {
                return m_x;
            }

            double getY() const
            {
                return m_y;
            }

        private:
            Button m_button;
            Action m_action;
            double m_x;
            double m_y;
    };

    class MouseMoveEvent
        : public Window::Event
    {
        public:
            MouseMoveEvent(
                double x,
                double y )
                : Window::Event( Window::Event::Type::MOUSE_MOVE )
                , m_x( x )
                , m_y( y )
            { }

            double getX() const
            {
                return m_x;
            }

            double getY() const
            {
                return m_y;
            }

        private:
            double m_x;
            double m_y;
    };
}
