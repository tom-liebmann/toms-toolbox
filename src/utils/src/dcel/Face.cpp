#include <ttb/utils/dcel/Face.hpp>

#include <ttb/utils/dcel/DCEL.hpp>
#include <ttb/utils/dcel/Data.hpp>
#include <ttb/utils/dcel/Edge.hpp>


namespace ttb::dcel
{
    FaceData::FaceData( EdgeId const& edgeId ) : m_edgeId( edgeId )
    {
    }


    FaceHandle::FaceHandle( DCEL& dcel, FaceId const& id ) : m_dcel( dcel ), m_id( id )
    {
    }

    DCEL& FaceHandle::dcel() const
    {
        return m_dcel;
    }

    FaceId const& FaceHandle::id() const
    {
        return m_id;
    }

    EdgeHandle FaceHandle::edge() const
    {
        return { m_dcel, m_dcel.m_data->faceData( m_id ).m_edgeId };
    }


    ConstFaceHandle::ConstFaceHandle( DCEL const& dcel, FaceId const& id )
        : m_dcel( dcel ), m_id( id )
    {
    }

    DCEL const& ConstFaceHandle::dcel() const
    {
        return m_dcel;
    }

    FaceId const& ConstFaceHandle::id() const
    {
        return m_id;
    }

    ConstEdgeHandle ConstFaceHandle::edge() const
    {
        return { m_dcel, m_dcel.m_data->faceData( m_id ).m_edgeId };
    }
}
