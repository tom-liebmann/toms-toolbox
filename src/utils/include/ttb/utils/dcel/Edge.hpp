#pragma once

#include "Dcel.hpp"

#include <utility>


namespace ttb::dcel
{
    template < typename DcelType >
    class EdgeHandle
    {
    public:
        EdgeHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        EdgeHandle( DcelType& dcel, EdgeId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        EdgeHandle( EdgeHandle< DcelType > const& rhs ) = default;

        EdgeHandle( EdgeHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( EdgeHandle< DcelType > const& rhs ) -> EdgeHandle< DcelType >& = default;

        auto operator=( EdgeHandle< DcelType >&& rhs ) -> EdgeHandle< DcelType >&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto getDCEL() const -> DcelType&
        {
            return *m_dcel;
        }

        auto getId() const -> VertexId
        {
            return m_id;
        }

        auto getData() const -> typename DcelType::EdgeData&
        {
            return getHolder().data;
        }

        auto getVertex() const -> VertexHandle< DcelType >
        {
            return { m_dcel, getHolder().vertex };
        }

        auto getFace() const -> FaceHandle< DcelType >
        {
            return { m_dcel, getHolder().face };
        }

        auto getTwin() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().twin };
        }

        auto getNext() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().next };
        }

        auto getPrev() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().prev };
        }

        auto setVertex( VertexHandle< DcelType > const& vertex ) const
            -> EdgeHandle< DcelType > const&
        {
            getHolder().vertex = vertex.getId();
            return *this;
        }

        auto setFace( FaceHandle< DcelType > const& face ) const -> EdgeHandle< DcelType > const&
        {
            getHolder().face = face.getId();
            return *this;
        }

        auto setTwin( EdgeHandle< DcelType > const& twin ) const -> EdgeHandle< DcelType > const&
        {
            getHolder().twin = twin.getId();
            return *this;
        }

        auto setNext( EdgeHandle< DcelType > const& next ) const -> EdgeHandle< DcelType > const&
        {
            getHolder().next = next.getId();
            return *this;
        }

        auto setPrev( EdgeHandle< DcelType > const& prev ) const -> EdgeHandle< DcelType > const&
        {
            getHolder().prev = prev.getId();
            return *this;
        }

    private:
        auto getHolder() const -> typename DcelType::EdgeHolder&
        {
            return m_dcel->m_edges.get( m_id );
        }

        DcelType* m_dcel;
        VertexId m_id;

        friend class ConstEdgeHandle< DcelType >;
    };

    template < typename DcelType >
    class ConstEdgeHandle
    {
    public:
        ConstEdgeHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        ConstEdgeHandle( DcelType const& dcel, EdgeId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        ConstEdgeHandle( ConstEdgeHandle< DcelType > const& rhs ) = default;

        ConstEdgeHandle( ConstEdgeHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        ConstEdgeHandle( EdgeHandle< DcelType > const& rhs )
            : m_dcel{ rhs.m_dcel }, m_id{ rhs.m_id }
        {
        }

        ConstEdgeHandle( EdgeHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( ConstEdgeHandle< DcelType > const& rhs ) -> ConstEdgeHandle< DcelType >& =
                                                                        default;

        auto operator=( ConstEdgeHandle< DcelType >&& rhs ) -> ConstEdgeHandle< DcelType >&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto operator=( EdgeHandle< DcelType > const& rhs ) -> ConstEdgeHandle< DcelType >&
        {
            m_dcel = rhs.m_dcel;
            m_id = rhs.m_id;
            return *this;
        }

        auto operator=( EdgeHandle< DcelType >&& rhs ) -> ConstEdgeHandle< DcelType >&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto getDCEL() const -> DcelType&
        {
            return *m_dcel;
        }

        auto getId() const -> VertexId
        {
            return m_id;
        }

        auto getData() const -> typename DcelType::EdgeData const&
        {
            return getHolder().data;
        }

        auto getVertex() const -> VertexHandle< DcelType >
        {
            return { m_dcel, getHolder().vertex };
        }

        auto getFace() const -> FaceHandle< DcelType >
        {
            return { m_dcel, getHolder().face };
        }

        auto getTwin() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().twin };
        }

        auto getNext() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().next };
        }

        auto getPrev() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().prev };
        }

    private:
        auto getHolder() const -> typename DcelType::EdgeHolder const&
        {
            return m_dcel->m_edges.get( m_id );
        }

        DcelType const* m_dcel;
        VertexId m_id;
    };
}
