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
    class VertexHandle
    {
    public:
        VertexHandle();

        VertexHandle( DCEL& dcel, VertexId const& id );

        VertexHandle( VertexHandle const& rhs ) = default;

        VertexHandle( VertexHandle&& rhs );

        VertexHandle& operator=( VertexHandle const& rhs ) = default;

        VertexHandle& operator=( VertexHandle&& rhs );

        DCEL& dcel() const;
        VertexId const& id() const;

        EdgeHandle edge() const;

        VertexHandle const& edge( EdgeId const& edgeId ) const;

    private:
        DCEL* m_dcel;
        VertexId m_id;

        friend class ConstVertexHandle;
    };


    class ConstVertexHandle
    {
    public:
        ConstVertexHandle();

        ConstVertexHandle( DCEL const& dcel, VertexId const& id );

        ConstVertexHandle( ConstVertexHandle const& rhs ) = default;

        ConstVertexHandle( ConstVertexHandle&& rhs );

        ConstVertexHandle( VertexHandle const& rhs );

        ConstVertexHandle( VertexHandle&& rhs );

        ConstVertexHandle& operator=( ConstVertexHandle const& rhs ) = default;

        ConstVertexHandle& operator=( ConstVertexHandle&& rhs );

        ConstVertexHandle& operator=( VertexHandle const& rhs );

        ConstVertexHandle& operator=( VertexHandle&& rhs );

        DCEL const& dcel() const;
        VertexId const& id() const;

        ConstEdgeHandle edge() const;

    private:
        DCEL const* m_dcel;
        VertexId m_id;
    };
}


namespace ttb::dcel
{
    inline VertexHandle::VertexHandle() : m_dcel{ nullptr }, m_id{ 0 }
    {
    }

    inline VertexHandle::VertexHandle( DCEL& dcel, VertexId const& id )
        : m_dcel{ &dcel }, m_id{ id }
    {
    }

    inline VertexHandle::VertexHandle( VertexHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline VertexHandle& VertexHandle::operator=( VertexHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline DCEL& VertexHandle::dcel() const
    {
        return *m_dcel;
    }

    inline VertexId const& VertexHandle::id() const
    {
        return m_id;
    }


    inline ConstVertexHandle::ConstVertexHandle() : m_dcel{ nullptr }, m_id{ 0 }
    {
    }

    inline ConstVertexHandle::ConstVertexHandle( DCEL const& dcel, VertexId const& id )
        : m_dcel{ &dcel }, m_id{ id }
    {
    }

    inline ConstVertexHandle::ConstVertexHandle( ConstVertexHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline ConstVertexHandle::ConstVertexHandle( VertexHandle const& rhs )
        : m_dcel{ rhs.m_dcel }, m_id{ rhs.m_id }
    {
    }

    inline ConstVertexHandle::ConstVertexHandle( VertexHandle&& rhs )
        : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
    {
    }

    inline ConstVertexHandle& ConstVertexHandle::operator=( ConstVertexHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline ConstVertexHandle& ConstVertexHandle::operator=( VertexHandle const& rhs )
    {
        m_dcel = rhs.m_dcel;
        m_id = rhs.m_id;
        return *this;
    }

    inline ConstVertexHandle& ConstVertexHandle::operator=( VertexHandle&& rhs )
    {
        m_dcel = std::exchange( rhs.m_dcel, nullptr );
        m_id = std::exchange( rhs.m_id, 0 );
        return *this;
    }

    inline DCEL const& ConstVertexHandle::dcel() const
    {
        return *m_dcel;
    }

    inline VertexId const& ConstVertexHandle::id() const
    {
        return m_id;
    }
}
