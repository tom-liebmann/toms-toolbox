#include <catch2/catch.hpp>

#include "Approx.hpp"
#include "TestElement.hpp"
#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Root.hpp>
#include <ttb/ui/elements/Group.hpp>
#include <ttb/utils/EventManager.hpp>
#include <ttb/ui/exceptions/VanishingElementException.hpp>

#include <fmt/core.h>


TEST_CASE( "Group", "[ui][elements]" )
{
    using namespace ttb::ui;

    auto resourceManager = std::shared_ptr< ttb::resources::Manager >();
    auto root = ttb::ui::Root{ resourceManager, { 1.0f, 1.0f } };

    SECTION( "Single child" )
    {
        auto group = Group{ root };
        group.setLeft( 0.1f );
        group.setTop( 0.2f );

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

    SECTION( "Vanishing element" )
    {
        auto group = Group{ root };

        auto child = ttb::TestElement{ root };
        child.setWidth( Extent::Type::MATCH_PARENT );
        child.setHeight( 1.0f );

        group.add( &child );
        REQUIRE_THROWS_AS( root.setChild( &group ), ttb::ui::VanishingElementException );
    }
}
