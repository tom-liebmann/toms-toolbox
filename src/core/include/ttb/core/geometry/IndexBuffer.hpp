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
        class Modifier;

        static std::shared_ptr< IndexBuffer > create();

        ~IndexBuffer();

        size_t size() const;

        std::shared_ptr< IndexBuffer > clone() const;

    private:
        IndexBuffer();

        IndexBuffer( IndexBuffer const& copy );

        GLuint m_bufferObject;
        std::vector< GLuint > m_data;

        friend class Geometry;
        friend Modifier modify( std::shared_ptr< IndexBuffer > buffer, size_t start );
        friend Modifier modify( std::shared_ptr< IndexBuffer > buffer );
    };


    class IndexBuffer::Modifier
    {
    public:
        size_t size() const;

        void reserve( size_t size );

        uint32_t& operator[]( size_t index );

        void resize( size_t size );

        void push_back( size_t value );

        template < typename... TType >
        void push_back( size_t value, TType... rest );

        std::shared_ptr< IndexBuffer > finish();

    private:
        Modifier( std::shared_ptr< IndexBuffer > buffer );

        void changed( size_t begin, size_t end );

        std::shared_ptr< IndexBuffer > m_buffer;
        size_t m_begin{ 0 };
        size_t m_end{ 0 };
        bool m_clear{ false };

        friend class IndexBuffer;
        friend Modifier modify( std::shared_ptr< IndexBuffer > buffer );
    };


    IndexBuffer::Modifier modify( std::shared_ptr< IndexBuffer > buffer );
}


// definitions
//=================================================================================================

namespace ttb
{
    template < typename... TType >
    void IndexBuffer::Modifier::push_back( size_t value, TType... rest )
    {
        push_back( value );
        push_back( rest... );
    }
}
