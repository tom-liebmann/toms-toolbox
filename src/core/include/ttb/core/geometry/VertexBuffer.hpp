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
    public:
        class Creator;
        class Modifier;
        class AttributeHandle;

        template < typename TFunc >
        static std::unique_ptr< VertexBuffer > create( TFunc const& func );

        template < typename TFunc >
        void modify( TFunc const& func );

        ~VertexBuffer();

        size_t size() const;

        std::unique_ptr< VertexBuffer > clone() const;

    private:
        struct Attribute
        {
            GLenum type;
            size_t size;
            size_t offset;

            size_t byteSize() const;
        };

        VertexBuffer( std::vector< Attribute > attributes );

        VertexBuffer( VertexBuffer const& copy );

        void bind( size_t index, GLint location ) const;

        GLuint m_bufferObject;
        std::vector< uint8_t > m_data;

        std::vector< Attribute > m_attributes;
        size_t m_blockSize;

        friend class Geometry;
    };


    class VertexBuffer::AttributeHandle
    {
    public:
        AttributeHandle& set( size_t index, float v0 );

        AttributeHandle& set( size_t index, float v0, float v1 );

        AttributeHandle& set( size_t index, float v0, float v1, float v2 );

        AttributeHandle& set( size_t index, float v0, float v1, float v2, float v3 );

        AttributeHandle& setComp( size_t index, size_t component, float value );

    private:
        AttributeHandle( VertexBuffer& buffer, size_t index );

        VertexBuffer& m_buffer;
        size_t m_index;

        friend class VertexBuffer;
    };


    class VertexBuffer::Modifier
    {
    public:
        Modifier( Modifier const& ) = delete;
        Modifier( Modifier&& ) = delete;

        Modifier& operator==( Modifier const& ) = delete;
        Modifier& operator==( Modifier&& ) = delete;

        void reserve( size_t elementCount );

        void resize( size_t elementCount );

        size_t size() const;

        void pop_back();

        void clear();

        AttributeHandle push_back();

        AttributeHandle operator[]( size_t index );

    private:
        Modifier( VertexBuffer& buffer );

        ~Modifier();

        void changed( size_t begin, size_t end );

        VertexBuffer& m_buffer;
        size_t m_begin{ 0 };
        size_t m_end{ 0 };
        bool m_clear{ false };

        friend class VertexBuffer;
    };


    class VertexBuffer::Creator
    {
    public:
        Creator( Creator const& ) = delete;
        Creator( Creator&& ) = delete;

        Creator& operator=( Creator const& ) = delete;
        Creator& operator=( Creator&& ) = delete;

        void attribute( GLenum type, size_t size );

    private:
        Creator();

        ~Creator();

        std::unique_ptr< VertexBuffer > finish();

        std::vector< Attribute > m_attributes;

        friend class VertexBuffer;
    };
}



// definitions
//=================================================================================================

namespace ttb
{
    template < typename TFunc >
    inline std::unique_ptr< VertexBuffer > VertexBuffer::create( TFunc const& func )
    {
        Creator creator;
        func( creator );
        return creator.finish();
    }

    template < typename TFunc >
    inline void VertexBuffer::modify( TFunc const& func )
    {
        Modifier modifier{ *this };
        func( modifier );
    }


    inline auto VertexBuffer::AttributeHandle::set( size_t index, float v0 ) -> AttributeHandle&
    {
        auto const offset = m_index * m_buffer.m_blockSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset ] ) = v0;
        return *this;
    }

    inline auto VertexBuffer::AttributeHandle::set( size_t index, float v0, float v1 )
        -> AttributeHandle&
    {
        auto const offset = m_index * m_buffer.m_blockSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 0 ] ) = v0;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 4 ] ) = v1;
        return *this;
    }

    inline auto VertexBuffer::AttributeHandle::set( size_t index, float v0, float v1, float v2 )
        -> AttributeHandle&
    {
        auto const offset = m_index * m_buffer.m_blockSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 0 ] ) = v0;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 4 ] ) = v1;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 8 ] ) = v2;
        return *this;
    }

    inline auto
        VertexBuffer::AttributeHandle::set( size_t index, float v0, float v1, float v2, float v3 )
            -> AttributeHandle&
    {
        auto const offset = m_index * m_buffer.m_blockSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 0 ] ) = v0;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 4 ] ) = v1;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 8 ] ) = v2;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 12 ] ) = v3;
        return *this;
    }

    inline auto VertexBuffer::AttributeHandle::setComp( size_t index,
                                                        size_t component,
                                                        float value ) -> AttributeHandle&
    {
        auto const offset = m_index * m_buffer.m_blockSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + component * 4 ] ) = value;
        return *this;
    }

    inline VertexBuffer::AttributeHandle::AttributeHandle( VertexBuffer& buffer, size_t index )
        : m_buffer{ buffer }, m_index{ index }
    {
    }
}
