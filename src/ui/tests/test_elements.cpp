#include <catch2/catch.hpp>

#include "FrameworkMock.hpp"
#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Framework.hpp>
#include <ttb/ui/elements/Center.hpp>
#include <ttb/ui/elements/Flex.hpp>
#include <ttb/ui/elements/Margin.hpp>
#include <ttb/ui/elements/Root.hpp>
#include <ttb/utils/EventManager.hpp>


namespace
{
    class TestElement : public ttb::ui::Element
    {
    public:
        TestElement( ttb::ui::Framework& framework,
                     std::optional< float > width,
                     std::optional< float > height )
            : Element{ framework }, m_width{ width }, m_height{ height }
        {
        }

        virtual Size fit( Size const& size ) override
        {
            return Size{ m_width.value_or( size( 0 ) ), m_height.value_or( size( 1 ) ) };
        }

        virtual void render( ttb::State& state ) const override
        {
        }

    private:
        std::optional< float > m_width;
        std::optional< float > m_height;
    };
}


namespace ttb
{
    template < typename TType >
    class Approx;

    template < typename TType, std::size_t TDim >
    class Approx< ttb::Vector< TType, TDim > >
    {
    public:
        Approx( ttb::Vector< TType, TDim > const& value ) : m_value{ value }
        {
        }

        bool operator==( ttb::Vector< TType, TDim > const& rhs ) const
        {
            for( std::size_t d = 0; d < TDim; ++d )
            {
                if( std::abs( rhs( d ) - m_value( d ) ) > m_epsilon )
                {
                    return false;
                }
            }

            return true;
        }

    private:
        TType m_epsilon{ 1e-5 };
        ttb::Vector< TType, TDim > m_value;
    };

    template < typename TType, std::size_t TDim >
    Approx( ttb::Vector< TType, TDim > const& ) -> Approx< ttb::Vector< TType, TDim > >;
}


TEST_CASE( "Center", "[ui][elements]" )
{
    auto framework = ttb::ui::test::FrameworkMock{};

    SECTION( "Centered" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto center = ttb::ui::Center{ framework,
                                       ttb::ui::Center::HAlignment::CENTER,
                                       ttb::ui::Center::VAlignment::MIDDLE };
        auto testElement = TestElement{ framework, 0.5f, 0.5f };

        center.child( &testElement );
        root.child( &center );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.size() );
        REQUIRE( ttb::Vector{ 0.25f, 0.25f } == testElement.offset() );

        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == center.offset() );
        REQUIRE( ttb::Vector{ 1.0f, 1.0f } == center.size() );
    }

    SECTION( "Right" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto center = ttb::ui::Center{ framework,
                                       ttb::ui::Center::HAlignment::RIGHT,
                                       ttb::ui::Center::VAlignment::BOTTOM };
        auto testElement = TestElement{ framework, 0.5f, 0.5f };

        center.child( &testElement );
        root.child( &center );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.size() );
        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.offset() );

        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == center.offset() );
        REQUIRE( ttb::Vector{ 1.0f, 1.0f } == center.size() );
    }

    SECTION( "Left" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto center = ttb::ui::Center{ framework,
                                       ttb::ui::Center::HAlignment::LEFT,
                                       ttb::ui::Center::VAlignment::TOP };
        auto testElement = TestElement{ framework, 0.5f, 0.5f };

        center.child( &testElement );
        root.child( &center );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == testElement.offset() );

        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == center.offset() );
        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == center.size() );
    }

    SECTION( "Min size" )
    {
        auto root = ttb::ui::Root{ framework, { 0.0f, 0.0f } };
        auto center = ttb::ui::Center{ framework,
                                       ttb::ui::Center::HAlignment::LEFT,
                                       ttb::ui::Center::VAlignment::TOP };
        auto testElement = TestElement{ framework, 0.5f, 0.5f };

        center.child( &testElement );
        root.child( &center );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == testElement.offset() );

        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == center.offset() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == center.size() );
    }
}

TEST_CASE( "Flex", "[ui][elements]" )
{
    auto framework = ttb::ui::test::FrameworkMock{};

    SECTION( "Single fixed" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto flex = ttb::ui::Flex{ framework, ttb::ui::Flex::Direction::VERTICAL };

        auto ele1 = TestElement{ framework, {}, {} };

        flex.addSlot( ttb::ui::Flex::SlotType::FIXED, 0.1f, &ele1 );
        root.child( &flex );

        REQUIRE( ttb::Vector{ 1.0f, 0.1f } == flex.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == flex.offset() );
        REQUIRE( ttb::Vector{ 1.0f, 0.1f } == ele1.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ele1.offset() );
    }

    SECTION( "Multiple fixed" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto flex = ttb::ui::Flex{ framework, ttb::ui::Flex::Direction::VERTICAL };

        auto ele1 = TestElement{ framework, {}, {} };
        auto ele2 = TestElement{ framework, {}, {} };

        flex.addSlot( ttb::ui::Flex::SlotType::FIXED, 0.1f, &ele1 );
        flex.addSlot( ttb::ui::Flex::SlotType::FIXED, 0.2f, &ele2 );
        root.child( &flex );

        REQUIRE( ttb::Vector{ 1.0f, 0.3f } == flex.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == flex.offset() );

        REQUIRE( ttb::Vector{ 1.0f, 0.1f } == ele1.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ele1.offset() );

        REQUIRE( ttb::Vector{ 1.0f, 0.2f } == ele2.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.1f } == ele2.offset() );
    }
}

TEST_CASE( "Flex & Margin", "[ui][flex][margin]" )
{
    auto framework = ttb::ui::test::FrameworkMock{};

    SECTION( "Fixed & Flex" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto margin = ttb::ui::Margin{ framework, 0.1f };
        auto flex = ttb::ui::Flex{ framework, ttb::ui::Flex::Direction::VERTICAL };

        auto ele1 = TestElement{ framework, {}, {} };
        auto ele2 = TestElement{ framework, {}, {} };

        flex.addSlot( ttb::ui::Flex::SlotType::FIXED, 0.1f, &ele1 );
        flex.addSlot( ttb::ui::Flex::SlotType::FLEX, 1.0f, &ele2 );
        margin.child( &flex );
        root.child( &margin );

        REQUIRE( ttb::Vector{ 0.8f, 0.8f } == ttb::Approx{ flex.size() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ flex.offset() } );

        REQUIRE( ttb::Vector{ 0.8f, 0.1f } == ttb::Approx{ ele1.size() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ttb::Approx{ ele1.offset() } );

        REQUIRE( ttb::Vector{ 0.8f, 0.7f } == ttb::Approx{ ele2.size() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.2f } == ttb::Approx{ ele2.offset() } );
    }
}

TEST_CASE( "Margin", "[ui][margin]" )
{
    auto framework = ttb::ui::test::FrameworkMock{};

    SECTION( "Simple margin" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto margin = ttb::ui::Margin{ framework, 0.1f };

        auto ele = TestElement{ framework, {}, {} };

        margin.child( &ele );
        root.child( &margin );

        REQUIRE( ttb::Vector{ 1.0f, 1.0f } == margin.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == margin.offset() );

        REQUIRE( ttb::Vector{ 0.8f, 0.8f } == ttb::Approx{ ele.size() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.1f } == ele.offset() );
    }
}
