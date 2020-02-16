#include <ttb/utils/dcel/Vertex.hpp>

#include <ttb/utils/dcel/DCEL.hpp>
#include <ttb/utils/dcel/Data.hpp>
#include <ttb/utils/dcel/Edge.hpp>


namespace ttb::dcel
{
    // VertexData
    //=================================================================================================

    VertexData::VertexData( EdgeId const& edgeId ) : m_edgeId( edgeId )
    {
    }


    // VertexHandle
    //=================================================================================================

    VertexHandle::VertexHandle( DCEL& dcel, VertexId const& id ) : m_dcel( dcel ), m_id( id )
    {
    }

    DCEL& VertexHandle::dcel() const
    {
        return m_dcel;
    }

    VertexId const& VertexHandle::id() const
    {
        return m_id;
    }

    EdgeHandle VertexHandle::edge() const
    {
        return { m_dcel, m_dcel.m_data->vertexData( m_id ).m_edgeId };
    }


    // ConstVertexHandle
    //=================================================================================================

    ConstVertexHandle::ConstVertexHandle( DCEL const& dcel, VertexId const& id )
        : m_dcel( dcel ), m_id( id )
    {
    }

    DCEL const& ConstVertexHandle::dcel() const
    {
        return m_dcel;
    }

    VertexId const& ConstVertexHandle::id() const
    {
        return m_id;
    }

    ConstEdgeHandle ConstVertexHandle::edge() const
    {
        return { m_dcel, m_dcel.m_data->vertexData( m_id ).m_edgeId };
    }
}
