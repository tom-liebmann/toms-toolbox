#pragma once

#include "ids.hpp"

#include <utility>


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
    class EdgeHandle
    {
    public:
        EdgeHandle();

        EdgeHandle( DCEL& dcel, EdgeId const& id );

        EdgeHandle( EdgeHandle const& rhs ) = default;

        EdgeHandle( EdgeHandle&& rhs );

        EdgeHandle& operator=( EdgeHandle const& rhs ) = default;

        EdgeHandle& operator=( EdgeHandle&& rhs );

        DCEL& dcel() const;
        EdgeId const& id() const;

        VertexHandle vertex() const;
        FaceHandle face() const;
        EdgeHandle twin() const;
        EdgeHandle next() const;
        EdgeHandle prev() const;

        EdgeHandle const& vertex( VertexId const& vertexId ) const;
        EdgeHandle const& face( FaceId const& faceId ) const;
        EdgeHandle const& twin( EdgeId const& edgeId ) const;
        EdgeHandle const& next( EdgeId const& edgeId ) const;
        EdgeHandle const& prev( EdgeId const& edgeId ) const;

    private:
        DCEL* m_dcel;
        EdgeId m_id;

        friend class ConstEdgeHandle;
    };


    bool operator==( EdgeHandle const& lhs, EdgeHandle const& rhs );

    bool operator!=( EdgeHandle const& lhs, EdgeHandle const& rhs );


    class ConstEdgeHandle
    {
    public:
        ConstEdgeHandle();

        ConstEdgeHandle( DCEL const& dcel, EdgeId const& id );

        ConstEdgeHandle( ConstEdgeHandle const& rhs ) = default;

        ConstEdgeHandle( ConstEdgeHandle&& rhs );

        ConstEdgeHandle( EdgeHandle const& rhs );

        ConstEdgeHandle( EdgeHandle&& rhs );

        ConstEdgeHandle& operator=( ConstEdgeHandle const& rhs ) = default;

        ConstEdgeHandle& operator=( ConstEdgeHandle&& rhs );

        ConstEdgeHandle& operator=( EdgeHandle const& rhs );

        ConstEdgeHandle& operator=( EdgeHandle&& rhs );

        DCEL const& dcel() const;
        EdgeId const& id() const;

        ConstVertexHandle vertex() const;
        ConstFaceHandle face() const;
        ConstEdgeHandle twin() const;
        ConstEdgeHandle next() const;
        ConstEdgeHandle prev() const;

    private:
        DCEL const* m_dcel;
        EdgeId m_id;
    };


    bool operator==( ConstEdgeHandle const& lhs, ConstEdgeHandle const& rhs );

    bool operator!=( ConstEdgeHandle const& lhs, ConstEdgeHandle const& rhs );
}


namespace ttb::dcel
{
    inline EdgeHandle::EdgeHandle() : m_dcel{ nullptr }, m_id{ 0 }
    {
    }

    inline EdgeHandle::EdgeHandle( DCEL& dcel, EdgeId const& id ) : m_dcel{ &dcel }, m_id{ id }
    {
    }

    inline EdgeHandle::EdgeHandle( EdgeHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline EdgeHandle& EdgeHandle::operator=( EdgeHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline DCEL& EdgeHandle::dcel() const
    {
        return *m_dcel;
    }

    inline EdgeId const& EdgeHandle::id() const
    {
        return m_id;
    }


    inline ConstEdgeHandle::ConstEdgeHandle() : m_dcel{ nullptr }, m_id{ 0 }
    {
    }

    inline ConstEdgeHandle::ConstEdgeHandle( DCEL const& dcel, EdgeId const& id )
        : m_dcel{ &dcel }, m_id{ id }
    {
    }

    inline ConstEdgeHandle::ConstEdgeHandle( ConstEdgeHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline ConstEdgeHandle::ConstEdgeHandle( EdgeHandle const& rhs )
        : m_dcel{ rhs.m_dcel }, m_id{ rhs.m_id }
    {
    }

    inline ConstEdgeHandle::ConstEdgeHandle( EdgeHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline ConstEdgeHandle& ConstEdgeHandle::operator=( ConstEdgeHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline ConstEdgeHandle& ConstEdgeHandle::operator=( EdgeHandle const& rhs )
    {
        m_dcel = rhs.m_dcel;
        m_id = rhs.m_id;
        return *this;
    }

    inline ConstEdgeHandle& ConstEdgeHandle::operator=( EdgeHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline DCEL const& ConstEdgeHandle::dcel() const
    {
        return *m_dcel;
    }

    inline EdgeId const& ConstEdgeHandle::id() const
    {
        return m_id;
    }
}
