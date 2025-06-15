#pragma once

#include <ttb/utils/LinkedVector.hpp>


namespace ttb
{
    template < typename VertexData, typename EdgeData, typename FaceData >
    class DCEL
    {
    public:
        using VertexId = std::size_t;
        using EdgeId = std::size_t;
        using FaceId = std::size_t;

        class VertexHandle;
        class ConstVertexHandle;
        class EdgeHandle;
        class ConstEdgeHandle;
        class FaceHandle;
        class ConstFaceHandle;

        auto getVertex( VertexId id ) -> VertexHandle
        {
            return { *this, id };
        }

        auto getVertex( VertexId id ) const -> ConstVertexHandle
        {
            return { *this, id };
        }

        auto getEdge( EdgeId id ) -> EdgeHandle
        {
            return { *this, id };
        }

        auto getEdge( EdgeId id ) const -> ConstEdgeHandle
        {
            return { *this, id };
        }

        auto getFace( FaceId id ) -> FaceHandle
        {
            return { *this, id };
        }

        auto getFace( FaceId id ) const -> ConstFaceHandle
        {
            return { *this, id };
        }

        auto addVertex( VertexData data ) -> VertexHandle
        {
            auto const id = m_vertices.add( VertexHolder{ std::move( data ) } );
            m_vertices.get( id ).id = id;
            return { *this, id };
        }

        auto addEdge( EdgeData data ) -> EdgeHandle
        {
            auto const id = m_edges.add( EdgeHolder{ std::move( data ) } );
            m_edges.get( id ).id = id;
            return { *this, id };
        }

        auto addFace( FaceData data ) -> FaceHandle
        {
            auto const id = m_faces.add( FaceHolder{ std::move( data ) } );
            m_faces.get( id ).id = id;
            return { *this, id };
        }

    private:
        struct VertexHolder
        {
            VertexId id;
            EdgeId edge;
            VertexData data;

            VertexHolder( VertexData dataValue ) : data{ std::move( dataValue ) }
            {
            }
        };

        struct EdgeHolder
        {
            EdgeId id;
            VertexId vertex;
            FaceId face;
            EdgeId twin;
            EdgeId next;
            EdgeId prev;
            EdgeData data;

            EdgeHolder( EdgeData dataValue ) : data{ std::move( dataValue ) }
            {
            }
        };

        struct FaceHolder
        {
            FaceId id;
            EdgeId edge;
            FaceData data;

            FaceHolder( FaceData dataValue ) : data{ std::move( dataValue ) }
            {
            }
        };

        LinkedVector< VertexHolder > m_vertices;
        LinkedVector< EdgeHolder > m_edges;
        LinkedVector< FaceHolder > m_faces;

        friend class VertexHandle;
        friend class ConstVertexHandle;
        friend class EdgeHandle;
        friend class ConstEdgeHandle;
        friend class FaceHandle;
        friend class ConstFaceHandle;
    };
}
