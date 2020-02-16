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

        virtual VertexData const& vertexData( VertexId const& id ) const = 0;

        virtual EdgeData const& edgeData( EdgeId const& id ) const = 0;

        virtual FaceData const& faceData( FaceId const& id ) const = 0;
    };
}
