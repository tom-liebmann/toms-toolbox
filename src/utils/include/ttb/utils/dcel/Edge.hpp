#pragma once

#include "ids.hpp"


namespace ttb::dcel
{
    class DCEL;
    class VertexHandle;
    class ConstVertexHandle;
    class FaceHandle;
    class ConstFaceHandle;
    class ConstEdgeHandle;
}


namespace ttb::dcel
{
    class EdgeData
    {
    public:
        EdgeData( VertexId vertexId, FaceId faceId, EdgeId twinId, EdgeId nextId, EdgeId prevId );

    private:
        VertexId m_vertexId;
        FaceId m_faceId;
        EdgeId m_twinId;
        EdgeId m_nextId;
        EdgeId m_prevId;

        friend class EdgeHandle;
        friend class ConstEdgeHandle;
    };


    class EdgeHandle
    {
    public:
        EdgeHandle( DCEL& dcel, EdgeId const& id );

        EdgeHandle( EdgeHandle const& copy );

        EdgeHandle& operator=( EdgeHandle const& copy );

        DCEL& dcel() const;
        EdgeId const& id() const;

        VertexHandle vertex() const;
        FaceHandle face() const;
        EdgeHandle twin() const;
        EdgeHandle next() const;
        EdgeHandle prev() const;

    private:
        DCEL& m_dcel;
        EdgeId m_id;

        friend class ConstEdgeHandle;
    };


    bool operator==( EdgeHandle const& lhs, EdgeHandle const& rhs );

    bool operator!=( EdgeHandle const& lhs, EdgeHandle const& rhs );


    class ConstEdgeHandle
    {
    public:
        ConstEdgeHandle( DCEL const& dcel, EdgeId const& id );

        ConstEdgeHandle( EdgeHandle const& copy );
        ConstEdgeHandle( ConstEdgeHandle const& copy );

        ConstEdgeHandle& operator=( EdgeHandle const& copy );
        ConstEdgeHandle& operator=( ConstEdgeHandle const& copy );

        DCEL const& dcel() const;
        EdgeId const& id() const;

        ConstVertexHandle vertex() const;
        ConstFaceHandle face() const;
        ConstEdgeHandle twin() const;
        ConstEdgeHandle next() const;
        ConstEdgeHandle prev() const;

    private:
        DCEL const& m_dcel;
        EdgeId m_id;
    };


    bool operator==( ConstEdgeHandle const& lhs, ConstEdgeHandle const& rhs );

    bool operator!=( ConstEdgeHandle const& lhs, ConstEdgeHandle const& rhs );
}
