#include <catch2/catch.hpp>

#include "FrameworkMock.hpp"
#include <ttb/ui/elements/XmlElement.hpp>


TEST_CASE( "Empty document", "[ui][xml]" )
{
    auto framework = ttb::ui::test::FrameworkMock{};

    rapidxml::xml_document<> doc;

    auto const element = ttb::ui::XmlElement{ framework, doc };
}

TEST_CASE( "Single element", "" )
{
    char data[] =
        "<center h='center' v='middle'>"
        "  <margin value='10'>"
        "    <rectangle color='#F00' />"
        "  </margin>"
        "</center>";

    auto framework = ttb::ui::test::FrameworkMock{};

    rapidxml::xml_document<> doc;
    doc.parse< 0 >( data );

    auto const element = ttb::ui::XmlElement{ framework, doc };
}
