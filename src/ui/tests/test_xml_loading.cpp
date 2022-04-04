#include <catch2/catch.hpp>

#include "FrameworkMock.hpp"
#include <ttb/ui/elements/Center.hpp>
#include <ttb/ui/elements/Margin.hpp>
#include <ttb/ui/elements/Rectangle.hpp>
#include <ttb/ui/elements/XmlElement.hpp>


TEST_CASE( "Empty document", "[ui][xml]" )
{
    auto framework = ttb::ui::test::FrameworkMock{};

    rapidxml::xml_document<> doc;

    auto const element = ttb::ui::XmlElement{ framework, doc };

    REQUIRE( element.m_elements.empty() );
}

TEST_CASE( "Single element", "" )
{
    char data[] =
        "<center h='center' v='middle'>"
        "  <margin value='10 3'>"
        "    <rectangle id='rect' color='#f00' />"
        "  </margin>"
        "</center>";

    auto framework = ttb::ui::test::FrameworkMock{};

    rapidxml::xml_document<> doc;
    doc.parse< 0 >( data );

    auto element = ttb::ui::XmlElement{ framework, doc };

    REQUIRE( element.m_elements.size() == 3 );

    REQUIRE( dynamic_cast< ttb::ui::Rectangle* >( element.m_elements[ 0 ].get() ) != nullptr );
    REQUIRE( dynamic_cast< ttb::ui::Margin* >( element.m_elements[ 1 ].get() ) != nullptr );
    REQUIRE( dynamic_cast< ttb::ui::Center* >( element.m_elements[ 2 ].get() ) != nullptr );

    REQUIRE( element.getElementById( "rect" ) == element.m_elements[ 0 ].get() );
    REQUIRE( element.getElementById( "invalid id" ) == nullptr );
}

TEST_CASE( "Load from source", "" )
{
    char const data[] = "<rectangle id='rect' color='#f00' />";

    auto framework = ttb::ui::test::FrameworkMock{};

    auto element = ttb::ui::XmlElement{ framework, data };

    REQUIRE( element.m_elements.size() == 1 );
    REQUIRE( dynamic_cast< ttb::ui::Rectangle* >( element.m_elements[ 0 ].get() ) != nullptr );
    REQUIRE( element.getTypeById< ttb::ui::Rectangle >( "rect" ) == element.m_elements[ 0 ].get() );
}
