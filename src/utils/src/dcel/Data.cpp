#include <ttb/utils/dcel/Data.hpp>

#include <stdexcept>


namespace ttb::dcel
{
    Data::~Data() = default;

    VertexId Data::addVertex()
    {
        throw std::runtime_error( "Operation not supported" );
    }

    EdgeId Data::addEdge()
    {
        throw std::runtime_error( "Operation not supported" );
    }

    FaceId Data::addFace()
    {
        throw std::runtime_error( "Operation not supported" );
    }

    void Data::removeVertex( VertexId const& /* vertexId */ )
    {
        throw std::runtime_error( "Operation not supported" );
    }

    void Data::removeEdge( EdgeId const& /* edgeId */ )
    {
        throw std::runtime_error( "Operation not supported" );
    }

    void Data::removeFace( FaceId const& /* faceId */ )
    {
        throw std::runtime_error( "Operation not supported" );
    }
}
