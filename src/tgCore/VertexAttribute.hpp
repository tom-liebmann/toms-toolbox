#pragma once

#include <string>

// declarations
//=============================================================================

namespace tg
{
    class VertexAttribute
    {
        public:
            VertexAttribute(
                std::string name,
                GLenum type,
                GLint size );

            const std::string& getName() const;
            GLenum getType() const;
            GLint getSize() const;

        private:
            std::string m_name;
            GLenum m_type;
            GLint m_size;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline VertexAttribute::VertexAttribute(
        std::string name,
        GLenum type,
        GLint size )
        : m_name( std::move( name ) )
        , m_type( type )
        , m_size( size )
    { }

    inline const std::string& VertexAttribute::getName() const
    {
        return m_name;
    }

    inline GLenum VertexAttribute::getType() const
    {
        return m_type;
    }

    inline GLint VertexAttribute::getSize() const
    {
        return m_size;
    }
}
