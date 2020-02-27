#include <ttb/utils/dcel/Vertex.hpp>

#include <ttb/utils/dcel/DCEL.hpp>
#include <ttb/utils/dcel/Data.hpp>
#include <ttb/utils/dcel/Edge.hpp>


namespace ttb::dcel
{
    // VertexHandle
    //=================================================================================================

    EdgeHandle VertexHandle::edge() const
    {
        return { *m_dcel, m_dcel->m_data->vertexEdge( m_id ) };
    }

    VertexHandle const& VertexHandle::edge( EdgeId const& edgeId ) const
    {
        m_dcel->m_data->vertexEdge( m_id, edgeId );
        return *this;
    }


    // ConstVertexHandle
    //=================================================================================================

    ConstEdgeHandle ConstVertexHandle::edge() const
    {
        return { *m_dcel, m_dcel->m_data->vertexEdge( m_id ) };
    }
}
