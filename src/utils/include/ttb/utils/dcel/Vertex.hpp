#pragma once

#include "DCEL.hpp"

#include <utility>


namespace ttb
{
    template < typename VertexData, typename EdgeData, typename FaceData >
    class DCEL< VertexData, EdgeData, FaceData >::VertexHandle
    {
    public:
        VertexHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        VertexHandle( DCEL& dcel, VertexId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        VertexHandle( VertexHandle const& rhs ) = default;

        VertexHandle( VertexHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( VertexHandle const& rhs ) -> VertexHandle& = default;

        auto operator=( VertexHandle&& rhs ) -> VertexHandle&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto getDCEL() const -> DCEL&
        {
            return *m_dcel;
        }

        auto getId() const -> VertexId
        {
            return m_id;
        }

        auto getData() const -> VertexData&
        {
            return getHolder().data;
        }

        auto getEdge() const -> EdgeHandle
        {
            return { m_dcel, getHolder().edge };
        }

        auto setEdge( EdgeHandle const& edge ) const -> VertexHandle const&
        {
            getHolder().edge = edge.getId();
            return *this;
        }

    private:
        auto getHolder() const -> VertexHolder&
        {
            return m_dcel->m_vertices.get( m_id );
        }

        DCEL* m_dcel;
        VertexId m_id;

        friend class ConstVertexHandle;
    };

    template < typename VertexData, typename EdgeData, typename FaceData >
    class DCEL< VertexData, EdgeData, FaceData >::ConstVertexHandle
    {
    public:
        ConstVertexHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        ConstVertexHandle( DCEL const& dcel, VertexId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        ConstVertexHandle( ConstVertexHandle const& rhs ) = default;

        ConstVertexHandle( ConstVertexHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        ConstVertexHandle( VertexHandle const& rhs ) : m_dcel{ rhs.m_dcel }, m_id{ rhs.m_id }
        {
        }

        ConstVertexHandle( VertexHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( ConstVertexHandle const& rhs ) -> ConstVertexHandle& = default;

        auto operator=( ConstVertexHandle&& rhs ) -> ConstVertexHandle&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto operator=( VertexHandle const& rhs ) -> ConstVertexHandle&
        {
            m_dcel = rhs.m_dcel;
            m_id = rhs.m_id;
            return *this;
        }

        auto operator=( VertexHandle&& rhs ) -> ConstVertexHandle&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto getDCEL() const -> DCEL&
        {
            return *m_dcel;
        }

        auto getId() const -> VertexId
        {
            return m_id;
        }

        auto getData() const -> VertexData const&
        {
            return getHolder().data;
        }

        auto getEdge() const -> EdgeHandle
        {
            return { m_dcel, getHolder().edge };
        }

    private:
        auto getHolder() const -> VertexHolder const&
        {
            return m_dcel->m_vertices.get( m_id );
        }

        DCEL const* m_dcel;
        VertexId m_id;
    };

    template < typename VertexData, typename EdgeData, typename FaceData >
    auto operator==( typename DCEL< VertexData, EdgeData, FaceData >::VertexHandle const& lhs,
                     typename DCEL< VertexData, EdgeData, FaceData >::VertexHandle const& rhs )
        -> bool
    {
        return lhs.getId() == rhs.getId();
    }

    template < typename VertexData, typename EdgeData, typename FaceData >
    auto operator!=( typename DCEL< VertexData, EdgeData, FaceData >::VertexHandle const& lhs,
                     typename DCEL< VertexData, EdgeData, FaceData >::VertexHandle const& rhs )
        -> bool
    {
        return lhs.getId() != rhs.getId();
    }
}
