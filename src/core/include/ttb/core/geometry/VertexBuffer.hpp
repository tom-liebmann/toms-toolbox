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
        class Access;

        static Creator create();

        ~VertexBuffer();

        std::shared_ptr< Access > access();

        void render( State& state ) const;

    private:
        VertexBuffer( std::vector< Attribute > attributes );

        void bind( size_t index, GLint location ) const;

        Access* m_access;

        GLuint m_bufferObject;
        std::vector< uint8_t > m_data;

        std::vector< Attribute > m_attributes;
        size_t m_blockSize;

        friend class Geometry;
    };



    class VertexBuffer::Access
    {
    public:
        template < typename... TType >
        size_t push( TType... data );

        void clear();

    private:
        static void finish( Access* access );

        Access( VertexBuffer& buffer );

        template < typename TType >
        void privPush( TType value );

        template < typename TType, typename... TTypes >
        void privPush( TType value, TTypes... rest );

        VertexBuffer& m_buffer;

        friend class VertexBuffer;
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

        std::unique_ptr< VertexBuffer > finish();

    private:
        Creator();

        std::vector< Attribute > m_attributes;

        friend class VertexBuffer;
    };
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
    size_t VertexBuffer::Access::push( TType... data )
    {
        size_t index = m_buffer.m_data.size() / m_buffer.m_blockSize;
        privPush( data... );
        return index;
    }

    template < typename TType >
    void VertexBuffer::Access::privPush( TType value )
    {
        using GLType = typename priv::gl_type< TType >::type;

        m_buffer.m_data.resize( m_buffer.m_data.size() + sizeof( GLType ) );
        *reinterpret_cast< GLType* >(
            &m_buffer.m_data[ m_buffer.m_data.size() - sizeof( GLType ) ] ) = value;
    }

    template < typename TType, typename... TTypes >
    void VertexBuffer::Access::privPush( TType value, TTypes... rest )
    {
        privPush( value );
        privPush( rest... );
    }
}
