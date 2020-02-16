#pragma once

#include "ids.hpp"


namespace ttb::dcel
{
    class DCEL;
    class EdgeHandle;
    class ConstEdgeHandle;
}


namespace ttb::dcel
{
    class VertexData
    {
    public:
        VertexData( EdgeId const& edgeId );

    private:
        EdgeId m_edgeId;

        friend class VertexHandle;
        friend class ConstVertexHandle;
    };


    class VertexHandle
    {
    public:
        VertexHandle( DCEL& dcel, VertexId const& id );

        DCEL& dcel() const;
        VertexId const& id() const;

        EdgeHandle edge() const;

    private:
        DCEL& m_dcel;
        VertexId m_id;
    };


    class ConstVertexHandle
    {
    public:
        ConstVertexHandle( DCEL const& dcel, VertexId const& id );

        DCEL const& dcel() const;
        VertexId const& id() const;

        ConstEdgeHandle edge() const;

    private:
        DCEL const& m_dcel;
        VertexId m_id;
    };
}
