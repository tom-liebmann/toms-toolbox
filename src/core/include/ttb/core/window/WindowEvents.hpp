#pragma once

#include <ttb/core/window/Window.hpp>

// declarations
//=============================================================================

namespace ttb
{
    namespace events
    {
        constexpr Event::Type KEY = 0;
        constexpr Event::Type MOUSE_BUTTON = 1;
        constexpr Event::Type MOUSE_MOVE = 2;
        constexpr Event::Type WINDOW_CLOSE = 3;
        constexpr Event::Type WINDOW_RESIZE = 4;
        constexpr Event::Type SCROLL = 5;



        class Key : public Event
        {
        public:
            enum class Action;

            Key( uint32_t key, Action action );

            uint32_t getKey() const;
            Action getAction() const;

            // Event
            virtual Type getType() const override;

        private:
            uint32_t m_key;
            Action m_action;
        };



        enum class Key::Action
        {
            DOWN,
            UP
        };



        class MouseButton : public Event
        {
        public:
            enum class Button;
            enum class Action;

            MouseButton( Button button, Action action, double x, double y );

            Button getButton() const;
            Action getAction() const;

            double getX() const;
            double getY() const;

            // Event
            virtual Type getType() const override;

        private:
            Button m_button;
            Action m_action;
            double m_x;
            double m_y;
        };



        enum class MouseButton::Button
        {
            LEFT,
            RIGHT,
            MIDDLE
        };



        enum class MouseButton::Action
        {
            UP,
            DOWN
        };



        class MouseMove : public Event
        {
        public:
            MouseMove( double x, double y );

            double getX() const;
            double getY() const;

            // Event
            virtual Type getType() const override;

        private:
            double m_x;
            double m_y;
        };



        class WindowClose : public Event
        {
        public:
            WindowClose( Window& window );

            Window& getWindow();

            // Event
            virtual Type getType() const override;

        private:
            Window& m_window;
        };



        class WindowResize : public Event
        {
        public:
            WindowResize( Window& window );

            Window const& window() const;

            // Event
            virtual Type getType() const override;

        private:
            Window& m_window;
        };



        class Scroll : public Event
        {
        public:
            Scroll( double xoffset, double yoffset );

            double getXOffset() const;
            double getYOffset() const;

            // Event
            virtual Type getType() const override;

        private:
            double m_xoffset;
            double m_yoffset;
        };
    }
}



// definitions
//=============================================================================

namespace ttb
{
    namespace events
    {
        inline Key::Key( uint32_t key, Action action ) : m_key( key ), m_action( action )
        {
        }

        inline uint32_t Key::getKey() const
        {
            return m_key;
        }

        inline Key::Action Key::getAction() const
        {
            return m_action;
        }

        inline Event::Type Key::getType() const
        {
            return KEY;
        }



        inline MouseButton::MouseButton( Button button, Action action, double x, double y )
            : m_button( button ), m_action( action ), m_x( x ), m_y( y )
        {
        }

        inline MouseButton::Button MouseButton::getButton() const
        {
            return m_button;
        }

        inline MouseButton::Action MouseButton::getAction() const
        {
            return m_action;
        }

        inline double MouseButton::getX() const
        {
            return m_x;
        }

        inline double MouseButton::getY() const
        {
            return m_y;
        }

        inline Event::Type MouseButton::getType() const
        {
            return MOUSE_BUTTON;
        }



        inline MouseMove::MouseMove( double x, double y ) : m_x( x ), m_y( y )
        {
        }

        inline double MouseMove::getX() const
        {
            return m_x;
        }

        inline double MouseMove::getY() const
        {
            return m_y;
        }

        inline Event::Type MouseMove::getType() const
        {
            return MOUSE_MOVE;
        }



        inline WindowClose::WindowClose( Window& window ) : m_window( window )
        {
        }

        inline Window& WindowClose::getWindow()
        {
            return m_window;
        }

        inline Event::Type WindowClose::getType() const
        {
            return WINDOW_CLOSE;
        }



        inline WindowResize::WindowResize( Window& window ) : m_window( window )
        {
        }

        inline Window const& WindowResize::window() const
        {
            return m_window;
        }

        inline Event::Type WindowResize::getType() const
        {
            return WINDOW_RESIZE;
        }



        inline Scroll::Scroll( double xoffset, double yoffset )
            : m_xoffset( xoffset ), m_yoffset( yoffset )
        {
        }

        inline double Scroll::getXOffset() const
        {
            return m_xoffset;
        }

        inline double Scroll::getYOffset() const
        {
            return m_yoffset;
        }

        inline Event::Type Scroll::getType() const
        {
            return SCROLL;
        }
    }
}
