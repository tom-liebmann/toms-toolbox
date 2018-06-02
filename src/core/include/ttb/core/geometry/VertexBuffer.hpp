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

        void render( State& state ) const;

    private:
        VertexBuffer( std::vector< Attribute > attributes );

        void bind( size_t index, GLint location ) const;

        GLuint m_bufferObject;
        std::vector< uint8_t > m_data;

        std::vector< Attribute > m_attributes;
        size_t m_blockSize;

        friend class Geometry;
        friend VertexBuffer::Modifier modify( std::shared_ptr< VertexBuffer > buffer,
                                              size_t start );
        friend VertexBuffer::Modifier modify( std::shared_ptr< VertexBuffer > buffer );
    };


    class VertexBuffer::Modifier
    {
    public:
        Modifier& reserve( size_t elementCount );

        template < typename... TType >
        Modifier& push( TType... data );

        // Cuts the buffer after the current position
        Modifier& trim();

        void finish();

    private:
        Modifier( std::shared_ptr< VertexBuffer > buffer, size_t start );

        template < typename TType >
        void privPush( TType value );

        template < typename TType, typename... TTypes >
        void privPush( TType value, TTypes... rest );

        std::shared_ptr< VertexBuffer > m_buffer;
        size_t m_begin;
        size_t m_end;
        bool m_clear;

        friend class VertexBuffer;
        friend VertexBuffer::Modifier modify( std::shared_ptr< VertexBuffer > buffer,
                                              size_t start );
        friend VertexBuffer::Modifier modify( std::shared_ptr< VertexBuffer > buffer );
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
    template < typename... TType >
    VertexBuffer::Modifier& VertexBuffer::Modifier::push( TType... data )
    {
        privPush( data... );
        return *this;
    }

    template < typename TType >
    void VertexBuffer::Modifier::privPush( TType value )
    {
        using GLType = typename priv::gl_type< TType >::type;

        if( m_buffer->m_data.size() < m_end + sizeof( GLType ) )
        {
            m_buffer->m_data.resize( m_end + sizeof( GLType ) );
            m_clear = true;
        }

        *reinterpret_cast< GLType* >( m_buffer->m_data.data() + m_end ) = value;
        m_end += sizeof( GLType );
    }

    template < typename TType, typename... TTypes >
    void VertexBuffer::Modifier::privPush( TType value, TTypes... rest )
    {
        privPush( value );
        privPush( rest... );
    }
}
