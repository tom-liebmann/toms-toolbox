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
    class FaceData
    {
    public:
        FaceData( EdgeId const& edgeId );

    private:
        EdgeId m_edgeId;

        friend class FaceHandle;
        friend class ConstFaceHandle;
    };


    class FaceHandle
    {
    public:
        FaceHandle( DCEL& dcel, FaceId const& id );

        DCEL& dcel() const;
        FaceId const& id() const;

        EdgeHandle edge() const;

    private:
        DCEL& m_dcel;
        FaceId m_id;
    };


    class ConstFaceHandle
    {
    public:
        ConstFaceHandle( DCEL const& dcel, FaceId const& id );

        DCEL const& dcel() const;
        FaceId const& id() const;

        ConstEdgeHandle edge() const;

    private:
        DCEL const& m_dcel;
        FaceId m_id;
    };
}
