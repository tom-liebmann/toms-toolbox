#pragma once

#include <ttb/core/gl.hpp>

#include <iostream>


// declarations
//=============================================================================

namespace ttb
{
    class Viewport
    {
    public:
        Viewport( GLint x, GLint y, GLsizei width, GLsizei height );

        GLint getX() const;
        GLint getY() const;
        GLsizei getWidth() const;
        GLsizei getHeight() const;

    private:
        GLint m_x;
        GLint m_y;
        GLsizei m_width;
        GLsizei m_height;
    };


    std::ostream& operator<<( std::ostream& stream, Viewport const& viewport );
}



// definitions
//=============================================================================

namespace ttb
{
    inline Viewport::Viewport( GLint x, GLint y, GLsizei width, GLsizei height )
        : m_x( x ), m_y( y ), m_width( width ), m_height( height )
    {
    }

    inline GLint Viewport::getX() const
    {
        return m_x;
    }

    inline GLint Viewport::getY() const
    {
        return m_y;
    }

    inline GLsizei Viewport::getWidth() const
    {
        return m_width;
    }

    inline GLsizei Viewport::getHeight() const
    {
        return m_height;
    }
}
