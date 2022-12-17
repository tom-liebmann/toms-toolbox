#include <fstream>
#include <ttb/core/geometry/ElementBatcher.hpp>


namespace ttb
{
    ElementBatcher::ElementBatcher( size_t verticesPerElement,
                                    size_t indicesPerElement,
                                    ttb::VertexBuffer& vertexBuffer,
                                    ttb::IndexBuffer& indexBuffer )
        : m_verticesPerElement{ verticesPerElement }
        , m_indicesPerElement{ indicesPerElement }
        , m_vertexBuffer{ vertexBuffer }
        , m_indexBuffer{ indexBuffer }
    {
    }

    auto ElementBatcher::createElement() -> Handle
    {
        auto const location = m_locationElements.size();

        auto const index = [ & ]
        {
            if( m_freeElements.empty() )
            {
                auto const index = m_elementLocations.size();
                m_elementLocations.push_back( location );
                return index;
            }
            else
            {
                auto const index = m_freeElements.front();
                m_freeElements.pop_front();
                m_elementLocations[ index ] = location;
                return index;
            }
        }();

        m_locationElements.push_back( index );

        m_vertexBuffer.resize( m_vertexBuffer.numVertices() + m_verticesPerElement );
        m_indexBuffer.resize( m_indexBuffer.numIndices() + m_indicesPerElement );

        return { *this, index };
    }

    auto ElementBatcher::element( size_t index ) -> Handle
    {
        return { *this, index };
    }

    void ElementBatcher::Handle::destroy()
    {
        if( m_batcher )
        {
            auto const newLocation = m_batcher->m_elementLocations[ m_index ];
            auto const oldLocation = m_batcher->m_locationElements.size() - 1;
            auto const index = m_batcher->m_locationElements.back();

            m_batcher->m_elementLocations[ index ] = newLocation;
            m_batcher->m_locationElements[ newLocation ] = index;
            m_batcher->m_locationElements.pop_back();

            // Copy indices to empty slot
            for( size_t i = 0; i < m_batcher->m_indicesPerElement; ++i )
            {
                auto& newIndex =
                    m_batcher->m_indexBuffer[ newLocation * m_batcher->m_indicesPerElement + i ];
                auto const oldIndex =
                    m_batcher->m_indexBuffer[ oldLocation * m_batcher->m_indicesPerElement + i ];
                newIndex = oldIndex + newLocation - oldLocation;
            }

            // Copy vertices to empty slot
            for( size_t i = 0; i < m_batcher->m_verticesPerElement; ++i )
            {
                auto newVertex =
                    m_batcher->m_vertexBuffer[ newLocation * m_batcher->m_verticesPerElement + i ];
                auto oldVertex =
                    m_batcher->m_vertexBuffer[ oldLocation * m_batcher->m_verticesPerElement + i ];
                newVertex.set( oldVertex );
            }

            m_batcher->m_vertexBuffer.resize( m_batcher->m_vertexBuffer.numVertices() -
                                              m_batcher->m_verticesPerElement );
            m_batcher->m_indexBuffer.resize( m_batcher->m_indexBuffer.numIndices() -
                                             m_batcher->m_indicesPerElement );

            m_batcher->m_freeElements.push_back( m_index );
            m_batcher->m_elementLocations[ m_index ] = INVALID_INDEX;

            m_batcher = nullptr;
            m_index = 0;
        }
    }

    bool ElementBatcher::Handle::isValid() const
    {
        if( !m_batcher )
        {
            // Handle was destroyed
            return false;
        }

        if( m_batcher->m_elementLocations[ m_index ] == INVALID_INDEX )
        {
            // Handle points to an empty element
            return false;
        }

        return true;
    }
}
