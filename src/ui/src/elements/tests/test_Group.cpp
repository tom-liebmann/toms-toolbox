#include <catch2/catch.hpp>

#include "Approx.hpp"
#include "TestElement.hpp"
#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Root.hpp>
#include <ttb/ui/elements/Group.hpp>
#include <ttb/utils/EventManager.hpp>

#include <fmt/core.h>


TEST_CASE( "Group", "[ui][elements]" )
{
    using namespace ttb::ui;

    auto resourceManager = std::shared_ptr< ttb::resources::Manager >();
    auto root = ttb::ui::Root{ resourceManager, { 1.0f, 1.0f } };

    SECTION( "Single child" )
    {
        auto group = Group{ root };
        group.setMargin( { 0.0f, 0.2f, 0.1f, 0.0f } );

        auto child = ttb::TestElement{ root };
        child.setWidth( 0.5f );
        child.setHeight( 0.4f );

        group.add( &child );
        root.setChild( &group );

        REQUIRE( ttb::Vector{ 0.5f, 0.4f } == ttb::Approx{ group.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.2f } == ttb::Approx{ group.getPosition() } );

        REQUIRE( ttb::Vector{ 0.5f, 0.4f } == ttb::Approx{ child.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.2f } == ttb::Approx{ child.getPosition() } );
    }

    SECTION( "Matching parent with single child" )
    {
        auto group = Group{ root };
        group.setMargin( { 0.0f, 0.2f, 0.1f, 0.0f } );

        auto child = ttb::TestElement{ root };
        child.setWidth( Extent::Type::MATCH_PARENT );
        child.setHeight( Extent::Type::MATCH_PARENT );
        child.setMargin( { 0.0f, 0.2f, 0.1f, 0.0f } );

        group.add( &child );
        root.setChild( &group );

        REQUIRE( group.getWidth().getType() == Extent::Type::MATCH_CHILD );
        REQUIRE( group.getHeight().getType() == Extent::Type::MATCH_CHILD );

        REQUIRE( ttb::Vector{ 0.9f, 0.8f } == ttb::Approx{ group.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.2f } == ttb::Approx{ group.getPosition() } );

        REQUIRE( ttb::Vector{ 0.8f, 0.6f } == ttb::Approx{ child.getSize() } );
        REQUIRE( ttb::Vector{ 0.2f, 0.4f } == ttb::Approx{ child.getPosition() } );
    }

    SECTION( "Multiple children" )
    {
        auto group = Group{ root };
        group.setMargin( { 0.1f } );

        auto child1 = ttb::TestElement{ root };
        child1.setWidth( 0.1f );
        child1.setHeight( 0.1f );
        child1.setMargin( { 0.1f } );

        auto child2 = ttb::TestElement{ root };
        child2.setWidth( 0.1f );
        child2.setHeight( 0.1f );
        child2.setMargin( { 0.2f } );

        group.add( &child1 );
        group.add( &child2 );
        root.setChild( &group );

        REQUIRE( group.getWidth().getType() == Extent::Type::MATCH_CHILD );
        REQUIRE( group.getHeight().getType() == Extent::Type::MATCH_CHILD );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == ttb::Approx{ group.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ group.getPosition() } );

        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ child1.getSize() } );
        REQUIRE( ttb::Vector{ 0.2f, 0.2f } == ttb::Approx{ child1.getPosition() } );

        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ child2.getSize() } );
        REQUIRE( ttb::Vector{ 0.3f, 0.3f } == ttb::Approx{ child2.getPosition() } );
    }

    SECTION( "Multiple children ignore size" )
    {
        auto group = Group{ root };
        group.setMargin( { 0.1f } );

        auto child1 = ttb::TestElement{ root };
        child1.setWidth( 0.1f );
        child1.setHeight( 0.1f );
        child1.setMargin( { 0.1f } );

        auto child2 = ttb::TestElement{ root };
        child2.setWidth( Extent::Type::MATCH_PARENT );
        child2.setHeight( Extent::Type::MATCH_PARENT );

        group.add( &child1 );
        group.add( &child2, false );
        root.setChild( &group );

        REQUIRE( group.getWidth().getType() == Extent::Type::MATCH_CHILD );
        REQUIRE( group.getHeight().getType() == Extent::Type::MATCH_CHILD );

        REQUIRE( ttb::Vector{ 0.3f, 0.3f } == ttb::Approx{ group.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ group.getPosition() } );

        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ child1.getSize() } );
        REQUIRE( ttb::Vector{ 0.2f, 0.2f } == ttb::Approx{ child1.getPosition() } );

        REQUIRE( ttb::Vector{ 0.3f, 0.3f } == ttb::Approx{ child2.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ child2.getPosition() } );
    }
}
