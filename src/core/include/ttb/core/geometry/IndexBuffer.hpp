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
        Modifier& reserve( size_t indexCount );

        Modifier& push( size_t index );

        template < typename... TType >
        Modifier& push( size_t index, TType... rest );

        Modifier& trim();

        std::shared_ptr< IndexBuffer > finish();

    private:
        Modifier( std::shared_ptr< IndexBuffer > buffer, size_t start );

        std::shared_ptr< IndexBuffer > m_buffer;
        size_t m_begin;
        size_t m_end;
        bool m_clear;

        friend class IndexBuffer;
        friend Modifier modify( std::shared_ptr< IndexBuffer > buffer, size_t start );
        friend Modifier modify( std::shared_ptr< IndexBuffer > buffer );
    };


    IndexBuffer::Modifier modify( std::shared_ptr< IndexBuffer > buffer, size_t start );

    IndexBuffer::Modifier modify( std::shared_ptr< IndexBuffer > buffer );
}


// definitions
//=================================================================================================

namespace ttb
{
    template < typename... TType >
    IndexBuffer::Modifier& IndexBuffer::Modifier::push( size_t index, TType... rest )
    {
        push( index );
        return push( rest... );
    }
}
