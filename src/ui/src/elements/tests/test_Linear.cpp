#include <catch2/catch.hpp>

#include "Approx.hpp"
#include "TestElement.hpp"
#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Root.hpp>
#include <ttb/ui/elements/Linear.hpp>
#include <ttb/utils/EventManager.hpp>

#include <fmt/core.h>


TEST_CASE( "Group", "[ui][elements]" )
{
    using namespace ttb::ui;

    auto resourceManager = std::shared_ptr< ttb::resources::Manager >();
    auto root = ttb::ui::Root{ resourceManager, { 1.0f, 1.0f } };

    SECTION( "Default Size" )
    {
        auto linear = Linear{ root, Linear::Direction::VERTICAL };

        REQUIRE( linear.getWidth().getType() == Extent::Type::MATCH_CHILD );
        REQUIRE( linear.getHeight().getType() == Extent::Type::MATCH_CHILD );
    }

    SECTION( "Single fixed child" )
    {
        auto linear = Linear{ root, Linear::Direction::VERTICAL };

        auto child = ttb::TestElement{ root };
        child.setWidth( 0.5f );
        child.setHeight( 0.5f );

        linear.add( &child );
        root.setChild( &linear );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == ttb::Approx{ linear.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ linear.getPosition() } );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == ttb::Approx{ child.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ child.getPosition() } );
    }

    SECTION( "Single matching child" )
    {
        auto linear = Linear{ root, Linear::Direction::VERTICAL };
        linear.setMargin( { 0.1f } );

        auto child = ttb::TestElement{ root };
        child.setWidth( Extent::Type::MATCH_PARENT );
        child.setHeight( Extent::Type::MATCH_PARENT );
        child.setMargin( { 0.1f } );

        linear.add( &child );
        root.setChild( &linear );

        REQUIRE( ttb::Vector{ 0.8f, 0.8f } == ttb::Approx{ linear.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ linear.getPosition() } );

        REQUIRE( ttb::Vector{ 0.6f, 0.6f } == ttb::Approx{ child.getSize() } );
        REQUIRE( ttb::Vector{ 0.2f, 0.2f } == ttb::Approx{ child.getPosition() } );
    }

    SECTION( "Two fixed children" )
    {
        auto linear = Linear{ root, Linear::Direction::VERTICAL };
        linear.setMargin( { 0.1f } );

        auto child1 = ttb::TestElement{ root };
        child1.setWidth( 0.1f );
        child1.setHeight( 0.1f );
        child1.setMargin( { 0.1f } );

        auto child2 = ttb::TestElement{ root };
        child2.setWidth( 0.2f );
        child2.setHeight( 0.2f );
        child2.setMargin( { 0.1f } );

        linear.add( &child1 );
        linear.add( &child2 );
        root.setChild( &linear );

        REQUIRE( ttb::Vector{ 0.4f, 0.7f } == ttb::Approx{ linear.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ linear.getPosition() } );

        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ child1.getSize() } );
        REQUIRE( ttb::Vector{ 0.2f, 0.2f } == ttb::Approx{ child1.getPosition() } );

        REQUIRE( ttb::Vector{ 0.2f, 0.2f } == ttb::Approx{ child2.getSize() } );
        REQUIRE( ttb::Vector{ 0.2f, 0.5f } == ttb::Approx{ child2.getPosition() } );
    }

    SECTION( "Two children - one matching" )
    {
        auto linear = Linear{ root, Linear::Direction::VERTICAL };
        linear.setMargin( { 0.1f } );

        auto child1 = ttb::TestElement{ root };
        child1.setWidth( Extent::Type::MATCH_PARENT );
        child1.setHeight( Extent::Type::MATCH_PARENT );
        child1.setMargin( { 0.1f } );

        auto child2 = ttb::TestElement{ root };
        child2.setWidth( 0.2f );
        child2.setHeight( 0.2f );
        child2.setMargin( { 0.1f } );

        linear.add( &child1 );
        linear.add( &child2 );
        root.setChild( &linear );

        REQUIRE( ttb::Vector{ 0.8f, 0.8f } == ttb::Approx{ linear.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ linear.getPosition() } );

        REQUIRE( ttb::Vector{ 0.6f, 0.2f } == ttb::Approx{ child1.getSize() } );
        REQUIRE( ttb::Vector{ 0.2f, 0.2f } == ttb::Approx{ child1.getPosition() } );

        REQUIRE( ttb::Vector{ 0.2f, 0.2f } == ttb::Approx{ child2.getSize() } );
        REQUIRE( ttb::Vector{ 0.2f, 0.6f } == ttb::Approx{ child2.getPosition() } );
    }

    SECTION( "Three children - different weights" )
    {
        auto linear = Linear{ root, Linear::Direction::VERTICAL };

        auto child1 = ttb::TestElement{ root };
        child1.setWidth( 0.2f );
        child1.setHeight( Extent::Type::MATCH_PARENT );

        auto child2 = ttb::TestElement{ root };
        child2.setWidth( 0.1f );
        child2.setHeight( 0.1f );

        auto child3 = ttb::TestElement{ root };
        child3.setWidth( 0.3f );
        child3.setHeight( Extent::Type::MATCH_PARENT );

        linear.add( &child1, 1.0f );
        linear.add( &child2 );
        linear.add( &child3, 2.0f );
        root.setChild( &linear );

        REQUIRE( ttb::Vector{ 0.3f, 1.0f } == ttb::Approx{ linear.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ linear.getPosition() } );

        REQUIRE( ttb::Vector{ 0.2f, 0.3f } == ttb::Approx{ child1.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ child1.getPosition() } );

        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ child2.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.3f } == ttb::Approx{ child2.getPosition() } );

        REQUIRE( ttb::Vector{ 0.3f, 0.6f } == ttb::Approx{ child3.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.4f } == ttb::Approx{ child3.getPosition() } );
    }
}
