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
        class Builder;

        using Index = GLushort;

        ~IndexBuffer();

        size_t numIndices() const;

        void reserve( size_t numIndices );

        void resize( size_t numIndices );

        void clear();

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


    class IndexBuffer::Builder
    {
    public:
        auto build() -> std::unique_ptr< IndexBuffer >;
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
