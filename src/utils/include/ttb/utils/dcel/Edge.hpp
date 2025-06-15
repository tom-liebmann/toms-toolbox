#pragma once

#include "DCEL.hpp"

#include <utility>


namespace ttb
{
    template < typename VertexData, typename EdgeData, typename FaceData >
    class DCEL< VertexData, EdgeData, FaceData >::EdgeHandle
    {
    public:
        EdgeHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        EdgeHandle( DCEL& dcel, EdgeId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        EdgeHandle( EdgeHandle const& rhs ) = default;

        EdgeHandle( EdgeHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( EdgeHandle const& rhs ) -> EdgeHandle& = default;

        auto operator=( EdgeHandle&& rhs ) -> EdgeHandle&
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

        auto getData() const -> EdgeData&
        {
            return getHolder().data;
        }

        auto getVertex() const -> VertexHandle
        {
            return { m_dcel, getHolder().vertex };
        }

        auto getFace() const -> FaceHandle
        {
            return { m_dcel, getHolder().face };
        }

        auto getTwin() const -> EdgeHandle
        {
            return { m_dcel, getHolder().twin };
        }

        auto getNext() const -> EdgeHandle
        {
            return { m_dcel, getHolder().next };
        }

        auto getPrev() const -> EdgeHandle
        {
            return { m_dcel, getHolder().prev };
        }

        auto setVertex( VertexHandle const& vertex ) const -> EdgeHandle const&
        {
            getHolder().vertex = vertex.getId();
            return *this;
        }

        auto setFace( FaceHandle const& face ) const -> EdgeHandle const&
        {
            getHolder().face = face.getId();
            return *this;
        }

        auto setTwin( EdgeHandle const& twin ) const -> EdgeHandle const&
        {
            getHolder().twin = twin.getId();
            return *this;
        }

        auto setNext( EdgeHandle const& next ) const -> EdgeHandle const&
        {
            getHolder().next = next.getId();
            return *this;
        }

        auto setPrev( EdgeHandle const& prev ) const -> EdgeHandle const&
        {
            getHolder().prev = prev.getId();
            return *this;
        }

    private:
        auto getHolder() const -> EdgeHolder&
        {
            return m_dcel->m_edges.get( m_id );
        }

        DCEL* m_dcel;
        VertexId m_id;

        friend class ConstEdgeHandle;
    };

    template < typename VertexData, typename EdgeData, typename FaceData >
    class DCEL< VertexData, EdgeData, FaceData >::ConstEdgeHandle
    {
    public:
        ConstEdgeHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        ConstEdgeHandle( DCEL const& dcel, EdgeId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        ConstEdgeHandle( ConstEdgeHandle const& rhs ) = default;

        ConstEdgeHandle( ConstEdgeHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        ConstEdgeHandle( EdgeHandle const& rhs ) : m_dcel{ rhs.m_dcel }, m_id{ rhs.m_id }
        {
        }

        ConstEdgeHandle( EdgeHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( ConstEdgeHandle const& rhs ) -> ConstEdgeHandle& = default;

        auto operator=( ConstEdgeHandle&& rhs ) -> ConstEdgeHandle&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto operator=( EdgeHandle const& rhs ) -> ConstEdgeHandle&
        {
            m_dcel = rhs.m_dcel;
            m_id = rhs.m_id;
            return *this;
        }

        auto operator=( EdgeHandle&& rhs ) -> ConstEdgeHandle&
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

        auto getData() const -> EdgeData const&
        {
            return getHolder().data;
        }

        auto getVertex() const -> VertexHandle
        {
            return { m_dcel, getHolder().vertex };
        }

        auto getFace() const -> FaceHandle
        {
            return { m_dcel, getHolder().face };
        }

        auto getTwin() const -> EdgeHandle
        {
            return { m_dcel, getHolder().twin };
        }

        auto getNext() const -> EdgeHandle
        {
            return { m_dcel, getHolder().next };
        }

        auto getPrev() const -> EdgeHandle
        {
            return { m_dcel, getHolder().prev };
        }

    private:
        auto getHolder() const -> EdgeHolder const&
        {
            return m_dcel->m_edges.get( m_id );
        }

        DCEL const* m_dcel;
        VertexId m_id;
    };

    template < typename VertexData, typename EdgeData, typename FaceData >
    auto
        operator==( typename DCEL< VertexData, EdgeData, FaceData >::EdgeHandle const& lhs,
                    typename DCEL< VertexData, EdgeData, FaceData >::EdgeHandle const& rhs ) -> bool
    {
        return lhs.getId() == rhs.getId();
    }

    template < typename VertexData, typename EdgeData, typename FaceData >
    auto
        operator!=( typename DCEL< VertexData, EdgeData, FaceData >::EdgeHandle const& lhs,
                    typename DCEL< VertexData, EdgeData, FaceData >::EdgeHandle const& rhs ) -> bool
    {
        return lhs.getId() != rhs.getId();
    }
}
