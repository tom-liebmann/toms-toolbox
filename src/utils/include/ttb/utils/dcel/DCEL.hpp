#pragma once

#include "ids.hpp"

#include <memory>


namespace ttb::dcel
{
    class Data;
    class VertexHandle;
    class ConstVertexHandle;
    class EdgeHandle;
    class ConstEdgeHandle;
    class FaceHandle;
    class ConstFaceHandle;
}


namespace ttb::dcel
{
    class DCEL
    {
    public:
        DCEL( std::unique_ptr< Data > data );

        ~DCEL();

        VertexHandle vertex( VertexId id );
        ConstVertexHandle vertex( VertexId id ) const;

        EdgeHandle edge( EdgeId id );
        ConstEdgeHandle edge( EdgeId id ) const;

        FaceHandle face( FaceId id );
        ConstFaceHandle face( FaceId id ) const;

        VertexHandle addVertex();

        EdgeHandle addEdge();

        FaceHandle addFace();

        template < typename TType >
        TType& data();

        template < typename TType >
        TType const& data() const;

        std::unique_ptr< Data > release();

    private:
        std::unique_ptr< Data > m_data;

        friend class VertexHandle;
        friend class ConstVertexHandle;
        friend class EdgeHandle;
        friend class ConstEdgeHandle;
        friend class FaceHandle;
        friend class ConstFaceHandle;
    };
}


namespace ttb::dcel
{
    template < typename TType >
    TType& DCEL::data()
    {
        return dynamic_cast< TType& >( *m_data );
    }

    template < typename TType >
    TType const& DCEL::data() const
    {
        return dynamic_cast< TType const& >( *m_data );
    }
}
