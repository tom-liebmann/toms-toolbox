#pragma once

#include "ids.hpp"


namespace ttb::dcel
{
    class EdgeData;
    class VertexData;
    class FaceData;
}


namespace ttb::dcel
{
    class Data
    {
    public:
        virtual ~Data();

        virtual VertexId edgeVertex( EdgeId const& id ) const = 0;
        virtual FaceId edgeFace( EdgeId const& id ) const = 0;
        virtual EdgeId edgeTwin( EdgeId const& id ) const = 0;
        virtual EdgeId edgeNext( EdgeId const& id ) const = 0;
        virtual EdgeId edgePrev( EdgeId const& id ) const = 0;

        virtual EdgeId faceEdge( FaceId const& id ) const = 0;

        virtual EdgeId vertexEdge( VertexId const& id ) const = 0;

        virtual void edgeVertex( EdgeId const& id, VertexId const& vertexId ) = 0;
        virtual void edgeFace( EdgeId const& id, FaceId const& faceId ) = 0;
        virtual void edgeTwin( EdgeId const& id, EdgeId const& edgeId ) = 0;
        virtual void edgeNext( EdgeId const& id, EdgeId const& edgeId ) = 0;
        virtual void edgePrev( EdgeId const& id, EdgeId const& edgeId ) = 0;

        virtual void faceEdge( FaceId const& id, EdgeId const& edgeId ) = 0;

        virtual void vertexEdge( VertexId const& id, EdgeId const& edgeId ) = 0;
    };
}
