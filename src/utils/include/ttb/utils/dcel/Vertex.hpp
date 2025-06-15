#pragma once

#include "Dcel.hpp"

#include <utility>


namespace ttb::dcel
{
    template < typename DcelType >
    class VertexHandle
    {
    public:
        VertexHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        VertexHandle( DcelType& dcel, VertexId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        VertexHandle( VertexHandle< DcelType > const& rhs ) = default;

        VertexHandle( VertexHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto
            operator=( VertexHandle< DcelType > const& rhs ) -> VertexHandle< DcelType >& = default;

        auto operator=( VertexHandle< DcelType >&& rhs ) -> VertexHandle< DcelType >&
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

        auto getData() const -> typename DcelType::VertexData&
        {
            return getHolder().data;
        }

        auto getEdge() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().edge };
        }

        auto setEdge( EdgeHandle< DcelType > const& edge ) const -> VertexHandle< DcelType > const&
        {
            getHolder().edge = edge.getId();
            return *this;
        }

    private:
        auto getHolder() const -> typename DcelType::VertexHolder&
        {
            return m_dcel->m_vertices.get( m_id );
        }

        DcelType* m_dcel;
        VertexId m_id;

        friend class ConstVertexHandle< DcelType >;
    };

    template < typename DcelType >
    class ConstVertexHandle
    {
    public:
        ConstVertexHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        ConstVertexHandle( DcelType const& dcel, VertexId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        ConstVertexHandle( ConstVertexHandle< DcelType > const& rhs ) = default;

        ConstVertexHandle( ConstVertexHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        ConstVertexHandle( VertexHandle< DcelType > const& rhs )
            : m_dcel{ rhs.m_dcel }, m_id{ rhs.m_id }
        {
        }

        ConstVertexHandle( VertexHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( ConstVertexHandle< DcelType > const& rhs )
            -> ConstVertexHandle< DcelType >& = default;

        auto operator=( ConstVertexHandle< DcelType >&& rhs ) -> ConstVertexHandle< DcelType >&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto operator=( VertexHandle< DcelType > const& rhs ) -> ConstVertexHandle< DcelType >&
        {
            m_dcel = rhs.m_dcel;
            m_id = rhs.m_id;
            return *this;
        }

        auto operator=( VertexHandle< DcelType >&& rhs ) -> ConstVertexHandle< DcelType >&
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

        auto getData() const -> typename DcelType::VertexData const&
        {
            return getHolder().data;
        }

        auto getEdge() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().edge };
        }

    private:
        auto getHolder() const -> typename DcelType::VertexHolder const&
        {
            return m_dcel->m_vertices.get( m_id );
        }

        DcelType const* m_dcel;
        VertexId m_id;
    };
}
