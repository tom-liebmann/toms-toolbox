#include <catch2/catch.hpp>

#include <ttb/utils/dcel.hpp>

struct EmptyData
{
};


TEST_CASE( "Empty DCEL", "[utils][dcel]" )
{
    auto dcel = ttb::DCEL< EmptyData, EmptyData, EmptyData >{};
}

TEST_CASE( "Simple triangle", "[utils][dcel]" )
{
    auto dcel = ttb::DCEL< EmptyData, EmptyData, EmptyData >{};

    auto v1 = dcel.addVertex( {} );
    auto v2 = dcel.addVertex( {} );
    auto v3 = dcel.addVertex( {} );

    auto e1 = dcel.addEdge( {} );
    auto e2 = dcel.addEdge( {} );
    auto e3 = dcel.addEdge( {} );

    auto e1t = dcel.addEdge( {} );
    auto e2t = dcel.addEdge( {} );
    auto e3t = dcel.addEdge( {} );

    auto f1 = dcel.addFace( {} );
    auto f2 = dcel.addFace( {} );

    v1.setEdge( e1 );
    v2.setEdge( e2 );
    v3.setEdge( e3 );

    e1.setVertex( v1 ).setFace( f1 ).setTwin( e1t ).setNext( e2 ).setPrev( e3 );
    e2.setVertex( v2 ).setFace( f1 ).setTwin( e2t ).setNext( e3 ).setPrev( e1 );
    e3.setVertex( v3 ).setFace( f1 ).setTwin( e3t ).setNext( e1 ).setPrev( e2 );

    e1t.setVertex( v2 ).setFace( f2 ).setTwin( e1 ).setNext( e3t ).setPrev( e2t );
    e2t.setVertex( v3 ).setFace( f2 ).setTwin( e2 ).setNext( e1t ).setPrev( e3t );
    e3t.setVertex( v1 ).setFace( f2 ).setTwin( e3 ).setNext( e2t ).setPrev( e1t );

    f1.setEdge( e1 );
    f2.setEdge( e1t );
}
