#include <ttb/utils/dcel/Edge.hpp>

#include <ttb/utils/dcel/DCEL.hpp>
#include <ttb/utils/dcel/Data.hpp>
#include <ttb/utils/dcel/Face.hpp>
#include <ttb/utils/dcel/Vertex.hpp>


namespace ttb::dcel
{
    // EdgeHandle
    //=================================================================================================

    VertexHandle EdgeHandle::vertex() const
    {
        return { *m_dcel, m_dcel->m_data->edgeVertex( m_id ) };
    }

    FaceHandle EdgeHandle::face() const
    {
        return { *m_dcel, m_dcel->m_data->edgeFace( m_id ) };
    }

    EdgeHandle EdgeHandle::twin() const
    {
        return { *m_dcel, m_dcel->m_data->edgeTwin( m_id ) };
    }

    EdgeHandle EdgeHandle::next() const
    {
        return { *m_dcel, m_dcel->m_data->edgeNext( m_id ) };
    }

    EdgeHandle EdgeHandle::prev() const
    {
        return { *m_dcel, m_dcel->m_data->edgePrev( m_id ) };
    }

    EdgeHandle const& EdgeHandle::vertex( VertexId const& vertexId ) const
    {
        m_dcel->m_data->edgeVertex( m_id, vertexId );
        return *this;
    }

    EdgeHandle const& EdgeHandle::face( FaceId const& faceId ) const
    {
        m_dcel->m_data->edgeFace( m_id, faceId );
        return *this;
    }

    EdgeHandle const& EdgeHandle::twin( EdgeId const& edgeId ) const
    {
        m_dcel->m_data->edgeTwin( m_id, edgeId );
        return *this;
    }

    EdgeHandle const& EdgeHandle::next( EdgeId const& edgeId ) const
    {
        m_dcel->m_data->edgeNext( m_id, edgeId );
        return *this;
    }

    EdgeHandle const& EdgeHandle::prev( EdgeId const& edgeId ) const
    {
        m_dcel->m_data->edgePrev( m_id, edgeId );
        return *this;
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

    ConstVertexHandle ConstEdgeHandle::vertex() const
    {
        return { *m_dcel, m_dcel->m_data->edgeVertex( m_id ) };
    }

    ConstFaceHandle ConstEdgeHandle::face() const
    {
        return { *m_dcel, m_dcel->m_data->edgeFace( m_id ) };
    }

    ConstEdgeHandle ConstEdgeHandle::twin() const
    {
        return { *m_dcel, m_dcel->m_data->edgeTwin( m_id ) };
    }

    ConstEdgeHandle ConstEdgeHandle::next() const
    {
        return { *m_dcel, m_dcel->m_data->edgeNext( m_id ) };
    }

    ConstEdgeHandle ConstEdgeHandle::prev() const
    {
        return { *m_dcel, m_dcel->m_data->edgePrev( m_id ) };
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
