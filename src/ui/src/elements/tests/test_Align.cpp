#include <catch2/catch.hpp>

#include "Approx.hpp"
#include "TestElement.hpp"
#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Root.hpp>
#include <ttb/ui/elements/Align.hpp>
#include <ttb/utils/EventManager.hpp>

#include <fmt/core.h>


TEST_CASE( "Align", "[ui][elements]" )
{
    using namespace ttb::ui;

    auto resourceManager = std::shared_ptr< ttb::resources::Manager >();
    auto root = ttb::ui::Root{ resourceManager, { 1.0f, 1.0f } };

    SECTION( "Top left alignment" )
    {
        auto align = Align{ root, Align::HAlignment::LEFT, Align::VAlignment::TOP };

        auto child = ttb::TestElement{ root };
        child.setWidth( 0.3f );
        child.setHeight( 0.3f );

        align.setChild( &child );
        root.setChild( &align );

        REQUIRE( ttb::Vector{ 0.3f, 0.3f } == ttb::Approx{ align.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ align.getPosition() } );

        REQUIRE( ttb::Vector{ 0.3f, 0.3f } == ttb::Approx{ child.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ child.getPosition() } );
    }

    SECTION( "Center middle alignment" )
    {
        auto align = Align{ root, Align::HAlignment::CENTER, Align::VAlignment::MIDDLE };

        auto child = ttb::TestElement{ root };
        child.setWidth( 0.2f );
        child.setHeight( 0.3f );

        align.setChild( &child );
        root.setChild( &align );

        REQUIRE( ttb::Vector{ 1.0f, 1.0f } == ttb::Approx{ align.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ align.getPosition() } );

        REQUIRE( ttb::Vector{ 0.2f, 0.3f } == ttb::Approx{ child.getSize() } );
        REQUIRE( ttb::Vector{ 0.4f, 0.35f } == ttb::Approx{ child.getPosition() } );
    }

    SECTION( "Right bottom alignment" )
    {
        auto align = Align{ root, Align::HAlignment::RIGHT, Align::VAlignment::BOTTOM };

        auto child = ttb::TestElement{ root };
        child.setWidth( 0.2f );
        child.setHeight( 0.3f );

        align.setChild( &child );
        root.setChild( &align );

        REQUIRE( ttb::Vector{ 1.0f, 1.0f } == ttb::Approx{ align.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ align.getPosition() } );

        REQUIRE( ttb::Vector{ 0.2f, 0.3f } == ttb::Approx{ child.getSize() } );
        REQUIRE( ttb::Vector{ 0.8f, 0.7f } == ttb::Approx{ child.getPosition() } );
    }
}
