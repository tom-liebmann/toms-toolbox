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
        class VertexHandle;

        template < typename TFunc >
        static std::unique_ptr< VertexBuffer > create( TFunc const& func );

        ~VertexBuffer();

        size_t numVertices() const;

        size_t byteSize() const;

        void reserve( size_t numVertices );

        void resize( size_t numVertices );

        void pop_back();

        void clear();

        VertexHandle push_back();

        VertexHandle operator[]( size_t index );

        /**
         * Flush the changes that are stored in this modified.
         * This also resets the modifier to the original state.
         */
        void flush();

    private:
        struct Attribute
        {
            GLenum type;
            size_t size;
            size_t offset;

            size_t byteSize() const;
        };

        VertexBuffer( std::vector< Attribute > attributes );

        VertexBuffer() = delete;
        VertexBuffer( VertexBuffer const& rhs ) = delete;
        VertexBuffer( VertexBuffer&& rhs ) = delete;
        VertexBuffer& operator=( VertexBuffer const& rhs ) = delete;
        VertexBuffer& operator=( VertexBuffer&& rhs ) = delete;

        void bind( size_t index, GLint location ) const;

        void changed( size_t begin, size_t end );

        size_t m_begin{ 0 };
        size_t m_end{ 0 };
        bool m_clear{ false };

        GLuint m_bufferObject;
        std::vector< uint8_t > m_data;

        std::vector< Attribute > m_attributes;
        size_t m_vertexSize;

        friend class Geometry;
    };


    class VertexBuffer::VertexHandle
    {
    public:
        VertexHandle& set( VertexHandle const& rhs );

        VertexHandle& set( size_t index, float v0 );

        VertexHandle& set( size_t index, float v0, float v1 );

        VertexHandle& set( size_t index, float v0, float v1, float v2 );

        VertexHandle& set( size_t index, float v0, float v1, float v2, float v3 );

        VertexHandle& setComp( size_t index, size_t component, float value );

    private:
        VertexHandle( VertexBuffer& buffer, size_t index );

        VertexBuffer& m_buffer;
        size_t m_index;

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


    inline auto VertexBuffer::VertexHandle::set( VertexHandle const& rhs ) -> VertexHandle&
    {
        std::copy( m_buffer.m_data.data() + rhs.m_index * m_buffer.m_vertexSize,
                   m_buffer.m_data.data() + ( rhs.m_index + 1 ) * m_buffer.m_vertexSize,
                   m_buffer.m_data.data() + m_index * m_buffer.m_vertexSize );
        return *this;
    }

    inline auto VertexBuffer::VertexHandle::set( size_t index, float v0 ) -> VertexHandle&
    {
        auto const offset = m_index * m_buffer.m_vertexSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset ] ) = v0;
        return *this;
    }

    inline auto VertexBuffer::VertexHandle::set( size_t index, float v0, float v1 ) -> VertexHandle&
    {
        auto const offset = m_index * m_buffer.m_vertexSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 0 ] ) = v0;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 4 ] ) = v1;
        return *this;
    }

    inline auto VertexBuffer::VertexHandle::set( size_t index, float v0, float v1, float v2 )
        -> VertexHandle&
    {
        auto const offset = m_index * m_buffer.m_vertexSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 0 ] ) = v0;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 4 ] ) = v1;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 8 ] ) = v2;
        return *this;
    }

    inline auto
        VertexBuffer::VertexHandle::set( size_t index, float v0, float v1, float v2, float v3 )
            -> VertexHandle&
    {
        auto const offset = m_index * m_buffer.m_vertexSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 0 ] ) = v0;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 4 ] ) = v1;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 8 ] ) = v2;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + 12 ] ) = v3;
        return *this;
    }

    inline auto VertexBuffer::VertexHandle::setComp( size_t index, size_t component, float value )
        -> VertexHandle&
    {
        auto const offset = m_index * m_buffer.m_vertexSize + m_buffer.m_attributes[ index ].offset;
        reinterpret_cast< float& >( m_buffer.m_data[ offset + component * 4 ] ) = value;
        return *this;
    }

    inline VertexBuffer::VertexHandle::VertexHandle( VertexBuffer& buffer, size_t index )
        : m_buffer{ buffer }, m_index{ index }
    {
    }
}
