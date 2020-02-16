#include <ttb/utils/dcel/Edge.hpp>

#include <ttb/utils/dcel/DCEL.hpp>
#include <ttb/utils/dcel/Data.hpp>
#include <ttb/utils/dcel/Face.hpp>
#include <ttb/utils/dcel/Vertex.hpp>


namespace ttb::dcel
{
    // EdgeData
    //=================================================================================================

    EdgeData::EdgeData(
        VertexId vertexId, FaceId faceId, EdgeId twinId, EdgeId nextId, EdgeId prevId )
        : m_vertexId( vertexId )
        , m_faceId( faceId )
        , m_twinId( twinId )
        , m_nextId( nextId )
        , m_prevId( prevId )
    {
    }


    // EdgeHandle
    //=================================================================================================

    EdgeHandle::EdgeHandle( DCEL& dcel, EdgeId const& id ) : m_dcel( dcel ), m_id( id )
    {
    }

    EdgeHandle::EdgeHandle( EdgeHandle const& copy ) : m_dcel( copy.m_dcel ), m_id( copy.m_id )
    {
    }

    EdgeHandle& EdgeHandle::operator=( EdgeHandle const& copy )
    {
        m_id = copy.m_id;
        return *this;
    }

    DCEL& EdgeHandle::dcel() const
    {
        return m_dcel;
    }

    EdgeId const& EdgeHandle::id() const
    {
        return m_id;
    }

    VertexHandle EdgeHandle::vertex() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_vertexId };
    }

    FaceHandle EdgeHandle::face() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_faceId };
    }

    EdgeHandle EdgeHandle::twin() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_twinId };
    }

    EdgeHandle EdgeHandle::next() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_nextId };
    }

    EdgeHandle EdgeHandle::prev() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_prevId };
    }


    bool operator==( EdgeHandle const& lhs, EdgeHandle const& rhs )
    {
        return lhs.id() == rhs.id();
    }

    bool operator!=( EdgeHandle const& lhs, EdgeHandle const& rhs )
    {
        return lhs.id() != rhs.id();
    }


    // ConstEdgeHandle
    //=================================================================================================

    ConstEdgeHandle::ConstEdgeHandle( DCEL const& dcel, EdgeId const& id )
        : m_dcel( dcel ), m_id( id )
    {
    }

    ConstEdgeHandle::ConstEdgeHandle( EdgeHandle const& copy )
        : m_dcel( copy.m_dcel ), m_id( copy.m_id )
    {
    }

    ConstEdgeHandle::ConstEdgeHandle( ConstEdgeHandle const& copy )
        : m_dcel( copy.m_dcel ), m_id( copy.m_id )
    {
    }

    ConstEdgeHandle& ConstEdgeHandle::operator=( EdgeHandle const& copy )
    {
        m_id = copy.m_id;
        return *this;
    }

    ConstEdgeHandle& ConstEdgeHandle::operator=( ConstEdgeHandle const& copy )
    {
        m_id = copy.m_id;
        return *this;
    }

    DCEL const& ConstEdgeHandle::dcel() const
    {
        return m_dcel;
    }

    EdgeId const& ConstEdgeHandle::id() const
    {
        return m_id;
    }

    ConstVertexHandle ConstEdgeHandle::vertex() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_vertexId };
    }

    ConstFaceHandle ConstEdgeHandle::face() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_faceId };
    }

    ConstEdgeHandle ConstEdgeHandle::twin() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_twinId };
    }

    ConstEdgeHandle ConstEdgeHandle::next() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_nextId };
    }

    ConstEdgeHandle ConstEdgeHandle::prev() const
    {
        return { m_dcel, m_dcel.m_data->edgeData( m_id ).m_prevId };
    }


    bool operator==( ConstEdgeHandle const& lhs, ConstEdgeHandle const& rhs )
    {
        return lhs.id() == rhs.id();
    }

    bool operator!=( ConstEdgeHandle const& lhs, ConstEdgeHandle const& rhs )
    {
        return lhs.id() != rhs.id();
    }
}
