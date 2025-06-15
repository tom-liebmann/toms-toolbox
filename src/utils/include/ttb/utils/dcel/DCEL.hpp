#pragma once

#include "ids.hpp"

#include <ttb/utils/LinkedVector.hpp>


namespace ttb::dcel
{
    template < typename DcelType >
    class VertexHandle;

    template < typename DcelType >
    class ConstVertexHandle;

    template < typename DcelType >
    class EdgeHandle;

    template < typename DcelType >
    class ConstEdgeHandle;

    template < typename DcelType >
    class FaceHandle;

    template < typename DcelType >
    class ConstFaceHandle;
}


namespace ttb::dcel
{
    template < typename TVertexData, typename TEdgeData, typename TFaceData >
    class DCEL
    {
    public:
        using VertexData = TVertexData;
        using EdgeData = TEdgeData;
        using FaceData = TFaceData;

        auto getVertex( VertexId id ) -> VertexHandle< DCEL >
        {
            return { *this, id };
        }

        auto getVertex( VertexId id ) const -> ConstVertexHandle< DCEL >
        {
            return { *this, id };
        }

        auto getEdge( EdgeId id ) -> EdgeHandle< DCEL >
        {
            return { *this, id };
        }

        auto getEdge( EdgeId id ) const -> ConstEdgeHandle< DCEL >
        {
            return { *this, id };
        }

        auto getFace( FaceId id ) -> FaceHandle< DCEL >
        {
            return { *this, id };
        }

        auto getFace( FaceId id ) const -> ConstFaceHandle< DCEL >
        {
            return { *this, id };
        }

        template < typename... TArgs >
        auto addVertex( TArgs&&... args ) -> VertexHandle< DCEL >
        {
            auto const id =
                m_vertices.add( VertexHolder{ VertexData{ std::forward< TArgs >( args )... } } );
            m_vertices.get( id ).id = id;
            return { *this, id };
        }

        template < typename... TArgs >
        auto addEdge( TArgs&&... args ) -> EdgeHandle< DCEL >
        {
            auto const id =
                m_edges.add( EdgeHolder{ EdgeData{ std::forward< TArgs >( args )... } } );
            m_edges.get( id ).id = id;
            return { *this, id };
        }

        template < typename... TArgs >
        auto addFace( TArgs&&... args ) -> FaceHandle< DCEL >
        {
            auto const id =
                m_faces.add( FaceHolder{ FaceData{ std::forward< TArgs >( args )... } } );
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

        friend class VertexHandle< DCEL >;
        friend class ConstVertexHandle< DCEL >;
        friend class EdgeHandle< DCEL >;
        friend class ConstEdgeHandle< DCEL >;
        friend class FaceHandle< DCEL >;
        friend class ConstFaceHandle< DCEL >;
    };
}
