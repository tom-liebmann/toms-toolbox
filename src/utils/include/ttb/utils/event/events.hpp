#pragma once

#include <ttb/utils/Event.hpp>


// declarations
//=============================================================================

namespace ttb::events
{
    class Key : public TypedEvent< ttb::event::type::KEY >
    {
    public:
        enum class Action;

        Key( uint32_t key, Action action );

        uint32_t getKey() const;
        Action getAction() const;

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


    class PointerDown : public TypedEvent< ttb::event::type::POINTER_DOWN >
    {
    public:
        PointerDown( PointerType pointerType, int pointerId, double x, double y );

        PointerType pointerType() const;

        int pointerId() const;

        double x() const;

        double y() const;

        virtual void transform( Transform const& transform,
                                EventCallback const& eventCallback ) const override;

    private:
        PointerType m_pointerType;
        int m_pointerId;
        double m_x;
        double m_y;
    };


    class PointerUp : public TypedEvent< ttb::event::type::POINTER_UP >
    {
    public:
        PointerUp( int pointerId, double x, double y );

        int pointerId() const;

        double x() const;

        double y() const;

        virtual void transform( Transform const& transform,
                                EventCallback const& eventCallback ) const override;

    private:
        int m_pointerId;
        double m_x;
        double m_y;
    };


    class PointerMove : public TypedEvent< ttb::event::type::POINTER_MOVE >
    {
    public:
        PointerMove( int pointerId, double x, double y );

        int pointerId() const;

        double x() const;

        double y() const;

        virtual void transform( Transform const& transform,
                                EventCallback const& eventCallback ) const override;

    private:
        int m_pointerId;
        double m_x;
        double m_y;
    };


    class Scroll : public TypedEvent< ttb::event::type::SCROLL >
    {
    public:
        Scroll( double xoffset, double yoffset );

        double getXOffset() const;
        double getYOffset() const;

    private:
        double m_xoffset;
        double m_yoffset;
    };
}



// definitions
//=============================================================================

namespace ttb::events
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


    inline PointerDown::PointerDown( PointerType pointerType, int pointerId, double x, double y )
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
}
