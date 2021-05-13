#include <ttb/core/geometry/ElementBatcher.hpp>


namespace ttb
{
    ElementBatcher::ElementBatcher( size_t verticesPerElement,
                                    size_t indicesPerElement,
                                    ttb::VertexBuffer::Modifier vertexBuffer,
                                    ttb::IndexBuffer::Modifier indexBuffer )
        : m_verticesPerElement{ verticesPerElement }
        , m_indicesPerElement{ indicesPerElement }
        , m_vertexBuffer{ std::move( vertexBuffer ) }
        , m_indexBuffer{ std::move( indexBuffer ) }
    {
    }

    auto ElementBatcher::createElement() -> Handle
    {
        auto const index = [ this ] {
            if( m_freeElements.empty() )
            {
                auto const index = m_elementLocations.size();
                m_elementLocations.push_back( 0 );
                return index;
            }
            else
            {
                auto const index = m_freeElements.front();
                m_freeElements.pop_front();
                return index;
            }
        }();

        m_elementLocations[ index ] = m_vertexBuffer.size();
        m_vertexBuffer.push_back();

        return { *this, index };
    }


    void ElementBatcher::Handle::destroy()
    {
        if( m_batcher )
        {
            auto const location = m_batcher->m_elementLocations[ m_index ];
            auto const oldLastLocation = m_batcher->m_elementLocations.back();
            m_batcher->m_elementLocations.back() = location;

            // Copy indices to empty slot
            for( size_t i = 0; i < m_batcher->m_indicesPerElement; ++i )
            {
                auto& newIndex =
                    m_batcher->m_indexBuffer[ location * m_batcher->m_indicesPerElement + i ];
                auto const oldIndex =
                    m_batcher
                        ->m_indexBuffer[ oldLastLocation * m_batcher->m_indicesPerElement + i ];
                newIndex = oldIndex + location - oldLastLocation;
            }

            // Copy vertices to empty slot
            for( size_t i = 0; i < m_batcher->m_verticesPerElement; ++i )
            {
                auto newVertex =
                    m_batcher->m_vertexBuffer[ location * m_batcher->m_verticesPerElement + i ];
                auto oldVertex =
                    m_batcher
                        ->m_vertexBuffer[ oldLastLocation * m_batcher->m_verticesPerElement + i ];
                newVertex.set( oldVertex );
            }

            m_batcher->m_vertexBuffer.pop_back();
            m_batcher->m_freeElements.push_back( m_index );

            m_batcher = nullptr;
            m_index = 0;
        }
    }
}
