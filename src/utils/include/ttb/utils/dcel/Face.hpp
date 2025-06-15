#pragma once

#include "DCEL.hpp"

#include <utility>


namespace ttb
{
    template < typename VertexData, typename EdgeData, typename FaceData >
    class DCEL< VertexData, EdgeData, FaceData >::FaceHandle
    {
    public:
        FaceHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        FaceHandle( DCEL& dcel, FaceId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        FaceHandle( FaceHandle const& rhs ) = default;

        FaceHandle( FaceHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( FaceHandle const& rhs ) -> FaceHandle& = default;

        auto operator=( FaceHandle&& rhs ) -> FaceHandle&
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

        auto getData() const -> FaceData&
        {
            return getHolder().data;
        }

        auto getEdge() const -> EdgeHandle
        {
            return { m_dcel, getHolder().edge };
        }

        auto setEdge( EdgeHandle const& edge ) const -> FaceHandle const&
        {
            getHolder().edge = edge.getId();
            return *this;
        }

    private:
        auto getHolder() const -> FaceHolder&
        {
            return m_dcel->m_faces.get( m_id );
        }

        DCEL* m_dcel;
        VertexId m_id;

        friend class ConstFaceHandle;
    };

    template < typename VertexData, typename EdgeData, typename FaceData >
    class DCEL< VertexData, EdgeData, FaceData >::ConstFaceHandle
    {
    public:
        ConstFaceHandle() : m_dcel{ nullptr }, m_id{ 0 }
        {
        }

        ConstFaceHandle( DCEL const& dcel, FaceId id ) : m_dcel{ &dcel }, m_id{ id }
        {
        }

        ConstFaceHandle( ConstFaceHandle const& rhs ) = default;

        ConstFaceHandle( ConstFaceHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        ConstFaceHandle( FaceHandle const& rhs ) : m_dcel{ rhs.m_dcel }, m_id{ rhs.m_id }
        {
        }

        ConstFaceHandle( FaceHandle&& rhs )
            : m_dcel{ std::exchange( rhs.m_dcel, nullptr ) }, m_id{ std::exchange( rhs.m_id, 0 ) }
        {
        }

        auto operator=( ConstFaceHandle const& rhs ) -> ConstFaceHandle& = default;

        auto operator=( ConstFaceHandle&& rhs ) -> ConstFaceHandle&
        {
            m_dcel = std::exchange( rhs.m_dcel, nullptr );
            m_id = std::exchange( rhs.m_id, 0 );
            return *this;
        }

        auto operator=( FaceHandle const& rhs ) -> ConstFaceHandle&
        {
            m_dcel = rhs.m_dcel;
            m_id = rhs.m_id;
            return *this;
        }

        auto operator=( FaceHandle&& rhs ) -> ConstFaceHandle&
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

        auto getData() const -> FaceData const&
        {
            return getHolder().data;
        }

        auto getEdge() const -> EdgeHandle
        {
            return { m_dcel, getHolder().edge };
        }

    private:
        auto getHolder() const -> FaceHolder const&
        {
            return m_dcel->m_faces.get( m_id );
        }

        DCEL const* m_dcel;
        VertexId m_id;
    };

    template < typename VertexData, typename EdgeData, typename FaceData >
    auto
        operator==( typename DCEL< VertexData, EdgeData, FaceData >::FaceHandle const& lhs,
                    typename DCEL< VertexData, EdgeData, FaceData >::FaceHandle const& rhs ) -> bool
    {
        return lhs.getId() == rhs.getId();
    }

    template < typename VertexData, typename EdgeData, typename FaceData >
    auto
        operator!=( typename DCEL< VertexData, EdgeData, FaceData >::FaceHandle const& lhs,
                    typename DCEL< VertexData, EdgeData, FaceData >::FaceHandle const& rhs ) -> bool
    {
        return lhs.getId() != rhs.getId();
    }
}
