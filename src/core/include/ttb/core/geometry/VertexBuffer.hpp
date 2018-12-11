#pragma once

#include <ttb/core/gl.hpp>

#include <iostream>
#include <memory>
#include <vector>


// forward declarations
//=============================================================================

namespace ttb
{
    class State;
}


// declarations
//=============================================================================

namespace ttb
{
    class VertexBuffer
    {
        class Creator;
        class Attribute;

    public:
        class Modifier;

        static Creator create();

        ~VertexBuffer();

        std::shared_ptr< VertexBuffer > clone() const;

    private:
        VertexBuffer( std::vector< Attribute > attributes );

        VertexBuffer( VertexBuffer const& copy );

        void bind( size_t index, GLint location ) const;

        GLuint m_bufferObject;
        std::vector< uint8_t > m_data;

        std::vector< Attribute > m_attributes;
        size_t m_blockSize;

        friend class Geometry;
        friend Modifier modify( std::shared_ptr< VertexBuffer > buffer, size_t start );
        friend Modifier modify( std::shared_ptr< VertexBuffer > buffer );
    };


    class VertexBuffer::Modifier
    {
    public:
        Modifier& reserve( size_t elementCount );

        template < typename TType >
        Modifier& push( TType value );

        template < typename TType, typename... TTypes >
        Modifier& push( TType value, TTypes... rest );

        // Cuts the buffer after the current position
        Modifier& trim();

        std::shared_ptr< VertexBuffer > finish();

    private:
        Modifier( std::shared_ptr< VertexBuffer > buffer, size_t start );

        std::shared_ptr< VertexBuffer > m_buffer;
        size_t m_begin;
        size_t m_end;
        bool m_clear;

        friend class VertexBuffer;
        friend Modifier modify( std::shared_ptr< VertexBuffer > buffer, size_t start );
        friend Modifier modify( std::shared_ptr< VertexBuffer > buffer );
    };


    class VertexBuffer::Attribute
    {
    public:
        Attribute( GLenum type, size_t size );

        GLenum type() const;

        size_t size() const;

        size_t byteSize() const;

    private:
        GLenum m_type;
        size_t m_size;
    };



    class VertexBuffer::Creator
    {
    public:
        Creator& attribute( GLenum type, size_t size );

        std::shared_ptr< VertexBuffer > finish();

    private:
        Creator();

        std::vector< Attribute > m_attributes;

        friend class VertexBuffer;
    };


    VertexBuffer::Modifier modify( std::shared_ptr< VertexBuffer > buffer, size_t start );

    VertexBuffer::Modifier modify( std::shared_ptr< VertexBuffer > buffer );
}



// definitions
//=================================================================================================

namespace ttb
{
    namespace priv
    {
        template < typename TType >
        struct gl_type;

        template <>
        struct gl_type< float >
        {
            using type = GLfloat;
        };

        template <>
        struct gl_type< double >
        {
            using type = GLfloat;
        };
    }
}


namespace ttb
{
    template < typename TType >
    VertexBuffer::Modifier& VertexBuffer::Modifier::push( TType value )
    {
        using GLType = typename priv::gl_type< TType >::type;

        auto& dat = m_buffer->m_data;

        if( dat.size() < m_end + sizeof( GLType ) )
        {
            m_clear = true;
        }

        GLType castedValue = value;
        auto valuePtr = reinterpret_cast< uint8_t const* >( &castedValue );

        dat.insert( std::end( dat ), valuePtr, valuePtr + sizeof( GLType ) );

        m_end += sizeof( GLType );

        return *this;
    }

    template < typename TType, typename... TTypes >
    VertexBuffer::Modifier& VertexBuffer::Modifier::push( TType value, TTypes... rest )
    {
        push( value );
        return push( rest... );
    }
}
