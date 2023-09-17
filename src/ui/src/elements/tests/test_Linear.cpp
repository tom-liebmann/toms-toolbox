#include <catch2/catch.hpp>

#include "Approx.hpp"
#include "TestElement.hpp"
#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Root.hpp>
#include <ttb/ui/elements/Linear.hpp>
#include <ttb/ui/exceptions/VanishingElementException.hpp>
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

        REQUIRE( linear.getWidth().getType() == Extent::Type::MATCH_PARENT );
        REQUIRE( linear.getHeight().getType() == Extent::Type::MATCH_CHILD );
    }
}
