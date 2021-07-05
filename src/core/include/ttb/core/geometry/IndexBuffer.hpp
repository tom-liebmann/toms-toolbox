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
        using Index = GLushort;

        static std::shared_ptr< IndexBuffer > create();

        ~IndexBuffer();

        size_t numIndices() const;

        void reserve( size_t numIndices );

        void resize( size_t numIndices );

        Index& operator[]( size_t index );

        void push_back( Index value );

        void pop_back();

        template < typename... TType >
        void push_back( Index value, TType... rest );

        void flush();

    private:
        IndexBuffer();

        IndexBuffer( IndexBuffer&& rhs ) = delete;
        IndexBuffer( IndexBuffer const& rhs ) = delete;
        IndexBuffer& operator=( IndexBuffer&& rhs ) = delete;
        IndexBuffer& operator=( IndexBuffer const& rhs ) = delete;

        GLuint m_bufferObject;
        std::vector< Index > m_data;

        void changed( size_t begin, size_t end );
        size_t m_begin{ 0 };
        size_t m_end{ 0 };
        bool m_clear{ false };

        friend class Geometry;
    };
}


// definitions
//=================================================================================================

namespace ttb
{
    template < typename... TType >
    void IndexBuffer::push_back( Index value, TType... rest )
    {
        push_back( value );
        push_back( rest... );
    }
}
