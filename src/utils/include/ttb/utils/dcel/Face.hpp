#pragma once

#include "ids.hpp"

#include <utility>


namespace ttb::dcel
{
    class DCEL;
    class EdgeHandle;
    class ConstEdgeHandle;
}


namespace ttb::dcel
{
    class FaceHandle
    {
    public:
        FaceHandle();

        FaceHandle( DCEL& dcel, FaceId const& id );

        FaceHandle( FaceHandle const& rhs ) = default;

        FaceHandle( FaceHandle&& rhs );

        FaceHandle& operator=( FaceHandle const& rhs ) = default;

        FaceHandle& operator=( FaceHandle&& rhs );

        DCEL& dcel() const;
        FaceId const& id() const;

        EdgeHandle edge() const;

        FaceHandle const& edge( EdgeId const& edgeId ) const;

    private:
        DCEL* m_dcel;
        FaceId m_id;

        friend class ConstFaceHandle;
    };


    class ConstFaceHandle
    {
    public:
        ConstFaceHandle();

        ConstFaceHandle( DCEL const& dcel, FaceId const& id );

        ConstFaceHandle( ConstFaceHandle const& rhs ) = default;

        ConstFaceHandle( ConstFaceHandle&& rhs );

        ConstFaceHandle( FaceHandle const& rhs );

        ConstFaceHandle( FaceHandle&& rhs );

        ConstFaceHandle& operator=( ConstFaceHandle const& rhs ) = default;

        ConstFaceHandle& operator=( ConstFaceHandle&& rhs );

        ConstFaceHandle& operator=( FaceHandle const& rhs );

        ConstFaceHandle& operator=( FaceHandle&& rhs );

        DCEL const& dcel() const;
        FaceId const& id() const;

        ConstEdgeHandle edge() const;

    private:
        DCEL const* m_dcel;
        FaceId m_id;
    };
}


namespace ttb::dcel
{
    inline FaceHandle::FaceHandle() : m_dcel{ nullptr }, m_id{ 0 }
    {
    }

    inline FaceHandle::FaceHandle( DCEL& dcel, FaceId const& id ) : m_dcel{ &dcel }, m_id{ id }
    {
    }

    inline FaceHandle::FaceHandle( FaceHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline FaceHandle& FaceHandle::operator=( FaceHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline DCEL& FaceHandle::dcel() const
    {
        return *m_dcel;
    }

    inline FaceId const& FaceHandle::id() const
    {
        return m_id;
    }


    inline ConstFaceHandle::ConstFaceHandle() : m_dcel{ nullptr }, m_id{ 0 }
    {
    }

    inline ConstFaceHandle::ConstFaceHandle( DCEL const& dcel, FaceId const& id )
        : m_dcel{ &dcel }, m_id{ id }
    {
    }

    inline ConstFaceHandle::ConstFaceHandle( ConstFaceHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline ConstFaceHandle::ConstFaceHandle( FaceHandle const& rhs )
    {
        m_dcel = rhs.m_dcel;
        m_id = rhs.m_id;
    }

    inline ConstFaceHandle::ConstFaceHandle( FaceHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline ConstFaceHandle& ConstFaceHandle::operator=( ConstFaceHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline ConstFaceHandle& ConstFaceHandle::operator=( FaceHandle const& rhs )
    {
        m_dcel = rhs.m_dcel;
        m_id = rhs.m_id;
        return *this;
    }

    inline ConstFaceHandle& ConstFaceHandle::operator=( FaceHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline DCEL const& ConstFaceHandle::dcel() const
    {
        return *m_dcel;
    }

    inline FaceId const& ConstFaceHandle::id() const
    {
        return m_id;
    }
}
