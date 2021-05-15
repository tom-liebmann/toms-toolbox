#pragma once

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

#include <cstdlib>
#include <list>


namespace ttb
{
    class ElementBatcher
    {
    public:
        class Handle;

        ElementBatcher( size_t verticesPerElement,
                        size_t indicesPerElement,
                        ttb::VertexBuffer& vertexBuffer,
                        ttb::IndexBuffer& indexBuffer );

        Handle createElement();

        Handle element( size_t index );

    private:
        /** Maps each element index to the location in the packed geometry buffers. */
        std::vector< size_t > m_elementLocations;

        std::list< size_t > m_freeElements;

        size_t m_verticesPerElement;

        size_t m_indicesPerElement;

        ttb::VertexBuffer& m_vertexBuffer;

        ttb::IndexBuffer& m_indexBuffer;
    };


    class ElementBatcher::Handle
    {
    public:
        Handle();

        Handle( Handle const& rhs );

        Handle( Handle&& rhs );

        void destroy();

        size_t index() const;

        VertexBuffer::VertexHandle vertex( size_t offset );

        void index( size_t offset, uint32_t value );

        Handle& operator=( Handle const& rhs );

        Handle& operator=( Handle&& rhs );

    private:
        Handle( ElementBatcher& batcher, size_t index );

        ElementBatcher* m_batcher{ nullptr };
        size_t m_index{ 0 };

        friend ElementBatcher;
    };


    inline ElementBatcher::Handle::Handle() = default;

    inline ElementBatcher::Handle::Handle( Handle const& rhs ) = default;

    inline ElementBatcher::Handle::Handle( Handle&& rhs )
        : m_batcher{ std::exchange( rhs.m_batcher, nullptr ) }
        , m_index{ std::exchange( rhs.m_index, 0 ) }
    {
    }

    inline size_t ElementBatcher::Handle::index() const
    {
        return m_index;
    }

    inline VertexBuffer::VertexHandle ElementBatcher::Handle::vertex( size_t offset )
    {
        auto const location = m_batcher->m_elementLocations[ m_index ];
        return m_batcher->m_vertexBuffer[ location * m_batcher->m_verticesPerElement + offset ];
    }

    inline void ElementBatcher::Handle::index( size_t offset, uint32_t value )
    {
        auto const location = m_batcher->m_elementLocations[ m_index ];
        m_batcher->m_indexBuffer[ location * m_batcher->m_indicesPerElement + offset ] =
            value + location * m_batcher->m_verticesPerElement;
    }

    inline auto ElementBatcher::Handle::operator=( Handle const& rhs ) -> Handle& = default;

    inline auto ElementBatcher::Handle::operator=( Handle&& rhs ) -> Handle&
    {
        m_batcher = std::exchange( rhs.m_batcher, nullptr );
        m_index = std::exchange( rhs.m_index, 0 );
        return *this;
    }

    inline ElementBatcher::Handle::Handle( ElementBatcher& batcher, size_t index )
        : m_batcher{ &batcher }, m_index{ index }
    {
    }
}
