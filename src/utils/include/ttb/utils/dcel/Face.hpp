#pragma once

#include "Dcel.hpp"

#include <utility>


namespace ttb::dcel
{
    template < typename DcelType >
    class FaceHandle
    {
    public:
        FaceHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        FaceHandle( DcelType& dcel, FaceId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        FaceHandle( FaceHandle< DcelType > const& rhs ) = default;

        FaceHandle( FaceHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( FaceHandle< DcelType > const& rhs ) -> FaceHandle< DcelType >& = default;

        auto operator=( FaceHandle< DcelType >&& rhs ) -> FaceHandle< DcelType >&
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

        auto getData() const -> typename DcelType::FaceData&
        {
            return getHolder().data;
        }

        auto getEdge() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().edge };
        }

        auto setEdge( EdgeHandle< DcelType > const& edge ) const -> FaceHandle< DcelType > const&
        {
            getHolder().edge = edge.getId();
            return *this;
        }

    private:
        auto getHolder() const -> typename DcelType::FaceHolder&
        {
            return m_dcel->m_faces.get( m_id );
        }

        DcelType* m_dcel;
        VertexId m_id;

        friend class ConstFaceHandle< DcelType >;
    };

    template < typename DcelType >
    class ConstFaceHandle
    {
    public:
        ConstFaceHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        ConstFaceHandle( DcelType const& dcel, FaceId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        ConstFaceHandle( ConstFaceHandle< DcelType > const& rhs ) = default;

        ConstFaceHandle( ConstFaceHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        ConstFaceHandle( FaceHandle< DcelType > const& rhs )
            : m_dcel{ rhs.m_dcel }, m_id{ rhs.m_id }
        {
        }

        ConstFaceHandle( FaceHandle< DcelType >&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( ConstFaceHandle< DcelType > const& rhs ) -> ConstFaceHandle< DcelType >& =
                                                                        default;

        auto operator=( ConstFaceHandle< DcelType >&& rhs ) -> ConstFaceHandle< DcelType >&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto operator=( FaceHandle< DcelType > const& rhs ) -> ConstFaceHandle< DcelType >&
        {
            m_dcel = rhs.m_dcel;
            m_id = rhs.m_id;
            return *this;
        }

        auto operator=( FaceHandle< DcelType >&& rhs ) -> ConstFaceHandle< DcelType >&
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

        auto getData() const -> typename DcelType::FaceData const&
        {
            return getHolder().data;
        }

        auto getEdge() const -> EdgeHandle< DcelType >
        {
            return { m_dcel, getHolder().edge };
        }

    private:
        auto getHolder() const -> typename DcelType::FaceHolder const&
        {
            return m_dcel->m_faces.get( m_id );
        }

        DcelType const* m_dcel;
        VertexId m_id;
    };
}
