#include <catch2/catch.hpp>

#include <ttb/utils/dcel.hpp>

struct EmptyData
{
};


TEST_CASE( "Empty DCEL", "[utils][dcel]" )
{
    auto dcel = ttb::dcel::Dcel< EmptyData, EmptyData, EmptyData >{};
}

TEST_CASE( "Simple triangle", "[utils][dcel]" )
{
    auto dcel = ttb::dcel::Dcel< EmptyData, EmptyData, EmptyData >{};

    auto v0 = dcel.addVertex();
    auto v1 = dcel.addVertex();
    auto v2 = dcel.addVertex();

    REQUIRE( v0 == v0 );
    REQUIRE( v1 == v1 );
    REQUIRE( v2 == v2 );

    REQUIRE( v0 != v1 );
    REQUIRE( v0 != v2 );
    REQUIRE( v1 != v2 );

    auto e0 = dcel.addEdge();
    auto e1 = dcel.addEdge();
    auto e2 = dcel.addEdge();

    auto e0t = dcel.addEdge();
    auto e1t = dcel.addEdge();
    auto e2t = dcel.addEdge();

    REQUIRE( e0 == e0 );
    REQUIRE( e1 == e1 );
    REQUIRE( e2 == e2 );

    REQUIRE( e0 != e1 );
    REQUIRE( e0 != e2 );
    REQUIRE( e1 != e2 );

    auto f0 = dcel.addFace();
    auto f1 = dcel.addFace();

    REQUIRE( f0 == f0 );
    REQUIRE( f1 == f1 );

    REQUIRE( f0 != f1 );

    v0.setEdge( e0 );
    v1.setEdge( e1 );
    v2.setEdge( e2 );

    e0.setVertex( v0 ).setFace( f0 ).setTwin( e0t ).setNext( e1 ).setPrev( e2 );
    e1.setVertex( v1 ).setFace( f0 ).setTwin( e1t ).setNext( e2 ).setPrev( e0 );
    e2.setVertex( v2 ).setFace( f0 ).setTwin( e2t ).setNext( e0 ).setPrev( e1 );

    e0t.setVertex( v1 ).setFace( f1 ).setTwin( e0 ).setNext( e2t ).setPrev( e1t );
    e1t.setVertex( v2 ).setFace( f1 ).setTwin( e1 ).setNext( e0t ).setPrev( e2t );
    e2t.setVertex( v0 ).setFace( f1 ).setTwin( e2 ).setNext( e1t ).setPrev( e0t );

    f0.setEdge( e0 );
    f1.setEdge( e0t );
}
