#include <ttb/utils/dcel/Face.hpp>

#include <ttb/utils/dcel/DCEL.hpp>
#include <ttb/utils/dcel/Data.hpp>
#include <ttb/utils/dcel/Edge.hpp>


namespace ttb::dcel
{
    EdgeHandle FaceHandle::edge() const
    {
        return { *m_dcel, m_dcel->m_data->faceEdge( m_id ) };
    }

    FaceHandle const& FaceHandle::edge( EdgeHandle const& edge ) const
    {
        m_dcel->m_data->faceEdge( m_id, edge.id() );
        return *this;
    }


    ConstEdgeHandle ConstFaceHandle::edge() const
    {
        return { *m_dcel, m_dcel->m_data->faceEdge( m_id ) };
    }
}
