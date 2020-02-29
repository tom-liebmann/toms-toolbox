#include <ttb/utils/dcel/DCEL.hpp>

#include <ttb/utils/dcel/Data.hpp>
#include <ttb/utils/dcel/Edge.hpp>
#include <ttb/utils/dcel/Face.hpp>
#include <ttb/utils/dcel/Vertex.hpp>


namespace ttb::dcel
{
    DCEL::DCEL( std::unique_ptr< Data > data ) : m_data( std::move( data ) )
    {
    }

    DCEL::~DCEL() = default;

    VertexHandle DCEL::vertex( VertexId id )
    {
        return { *this, id };
    }

    ConstVertexHandle DCEL::vertex( VertexId id ) const
    {
        return { *this, id };
    }

    EdgeHandle DCEL::edge( EdgeId id )
    {
        return { *this, id };
    }

    ConstEdgeHandle DCEL::edge( EdgeId id ) const
    {
        return { *this, id };
    }

    FaceHandle DCEL::face( FaceId id )
    {
        return { *this, id };
    }

    ConstFaceHandle DCEL::face( FaceId id ) const
    {
        return { *this, id };
    }

    VertexHandle DCEL::addVertex()
    {
        return { *this, m_data->addVertex() };
    }

    EdgeHandle DCEL::addEdge()
    {
        return { *this, m_data->addEdge() };
    }

    FaceHandle DCEL::addFace()
    {
        return { *this, m_data->addFace() };
    }

    std::unique_ptr< Data > DCEL::release()
    {
        return std::move( m_data );
    }
}
