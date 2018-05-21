#pragma once

#include <ttb/core/gl.hpp>

#include <memory>
#include <vector>

// declarations
//=================================================================================================

namespace ttb
{
    class IndexBuffer
    {
    public:
        class Access;

        IndexBuffer();

        ~IndexBuffer();

        std::shared_ptr< Access > access();

    private:
        Access* m_access;

        GLuint m_bufferObject;
        std::vector< GLuint > m_data;

        friend class Geometry;
    };



    class IndexBuffer::Access
    {
    public:
        template < typename... TType >
        void push( size_t index, TType... rest );

        void push( size_t index );

        void clear();

    private:
        Access( IndexBuffer& buffer );

        static void finish( Access* access );

        IndexBuffer& m_buffer;

        friend class IndexBuffer;
    };
}



// definitions
//=================================================================================================

namespace ttb
{
    template < typename... TType >
    void IndexBuffer::Access::push( size_t index, TType... rest )
    {
        push( index );
        push( rest... );
    }
}
