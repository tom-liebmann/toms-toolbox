#pragma once

#include <ttb/core/window/Window.hpp>

// declarations
//=============================================================================

namespace ttb
{
    namespace events
    {
        constexpr Event::Type KEY = 0;
        constexpr Event::Type POINTER_DOWN = 1;
        constexpr Event::Type POINTER_UP = 2;
        constexpr Event::Type POINTER_MOVE = 3;
        constexpr Event::Type WINDOW_CLOSE = 4;
        constexpr Event::Type WINDOW_RESIZE = 5;
        constexpr Event::Type SCROLL = 6;



        class Key : public Event
        {
        public:
            enum class Action;

            Key( uint32_t key, Action action );

            uint32_t getKey() const;
            Action getAction() const;

            // Event
            virtual Type type() const override;

        private:
            uint32_t m_key;
            Action m_action;
        };



        enum class Key::Action
        {
            DOWN,
            UP,
            UNKNOWN
        };


        enum class PointerType
        {
            UNKNOWN,
            MOUSE,
            FINGER,
        };


        class PointerDown : public TypedEvent< POINTER_DOWN >
        {
        public:
            PointerDown( PointerType pointerType, int pointerId, double x, double y );

            PointerType pointerType() const;

            int pointerId() const;

            double x() const;

            double y() const;

        private:
            PointerType m_pointerType;
            int m_pointerId;
            double m_x;
            double m_y;
        };


        class PointerUp : public TypedEvent< POINTER_UP >
        {
        public:
            PointerUp( int pointerId, double x, double y );

            int pointerId() const;

            double x() const;

            double y() const;

        private:
            int m_pointerId;
            double m_x;
            double m_y;
        };


        class PointerMove : public TypedEvent< POINTER_MOVE >
        {
        public:
            PointerMove( int pointerId, double x, double y );

            int pointerId() const;

            double x() const;

            double y() const;

        private:
            int m_pointerId;
            double m_x;
            double m_y;
        };


        class WindowClose : public Event
        {
        public:
            WindowClose( Window& window );

            Window& getWindow();

            // Event
            virtual Type type() const override;

        private:
            Window& m_window;
        };



        class WindowResize : public Event
        {
        public:
            WindowResize( Window& window );

            Window const& window() const;

            // Event
            virtual Type type() const override;

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
            virtual Type type() const override;

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

        inline Event::Type Key::type() const
        {
            return KEY;
        }


        inline PointerDown::PointerDown( PointerType pointerType,
                                         int pointerId,
                                         double x,
                                         double y )
            : m_pointerType{ pointerType }, m_pointerId{ pointerId }, m_x{ x }, m_y{ y }
        {
        }

        inline PointerType PointerDown::pointerType() const
        {
            return m_pointerType;
        }

        inline int PointerDown::pointerId() const
        {
            return m_pointerId;
        }

        inline double PointerDown::x() const
        {
            return m_x;
        }

        inline double PointerDown::y() const
        {
            return m_y;
        }


        inline PointerUp::PointerUp( int pointerId, double x, double y )
            : m_pointerId{ pointerId }, m_x{ x }, m_y{ y }
        {
        }

        inline int PointerUp::pointerId() const
        {
            return m_pointerId;
        }

        inline double PointerUp::x() const
        {
            return m_x;
        }

        inline double PointerUp::y() const
        {
            return m_y;
        }


        inline PointerMove::PointerMove( int pointerId, double x, double y )
            : m_pointerId{ pointerId }, m_x{ x }, m_y{ y }
        {
        }

        inline int PointerMove::pointerId() const
        {
            return m_pointerId;
        }

        inline double PointerMove::x() const
        {
            return m_x;
        }

        inline double PointerMove::y() const
        {
            return m_y;
        }


        inline WindowClose::WindowClose( Window& window ) : m_window( window )
        {
        }

        inline Window& WindowClose::getWindow()
        {
            return m_window;
        }

        inline Event::Type WindowClose::type() const
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

        inline Event::Type WindowResize::type() const
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

        inline Event::Type Scroll::type() const
        {
            return SCROLL;
        }
    }
}
