#include <catch2/catch.hpp>

#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Framework.hpp>
#include <ttb/ui/elements/Center.hpp>
#include <ttb/ui/elements/Root.hpp>
#include <ttb/utils/EventManager.hpp>


namespace
{
    class TestElement : public ttb::ui::Element
    {
    public:
        TestElement( ttb::ui::Framework& framework,
                     bool fixedWidth,
                     float width,
                     bool fixedHeight,
                     float height )
            : Element{ framework }
            , m_fixedWidth{ fixedWidth }
            , m_width{ width }
            , m_fixedHeight{ fixedHeight }
            , m_height{ height }
        {
        }

        virtual Size fit( Size const& size ) override
        {
            return Element::fit(
                { m_fixedWidth ? m_width : size( 0 ), m_fixedHeight ? m_height : size( 1 ) } );
        }

        virtual void render( ttb::State& /* state */ ) const override
        {
        }

    private:
        bool m_fixedWidth;
        float m_width;

        bool m_fixedHeight;
        float m_height;
    };

    class Framework : public ttb::ui::Framework
    {
    public:
        virtual ttb::resources::Manager& resourceManager() override
        {
            return m_resourceManager;
        }

        virtual ttb::EventManager& eventManager() override
        {
            return m_eventManager;
        }

    private:
        ttb::resources::Manager m_resourceManager;
        ttb::EventManager m_eventManager;
    };
}


TEST_CASE( "Center", "[ui][elements]" )
{
    auto framework = Framework{};

    SECTION( "Centered" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto center = ttb::ui::Center{ framework,
                                       ttb::ui::Center::HAlignment::CENTER,
                                       ttb::ui::Center::VAlignment::MIDDLE };
        auto testElement = TestElement{ framework, true, 0.5f, true, 0.5f };

        center.child( &testElement );
        root.child( &center );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.size() );
        REQUIRE( ttb::Vector{ 0.25f, 0.25f } == testElement.localToScreen( { 0.0f, 0.0f } ) );
        REQUIRE( ttb::Vector{ 0.75f, 0.75f } == center.size() );
    }

    SECTION( "Right" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto center = ttb::ui::Center{ framework,
                                       ttb::ui::Center::HAlignment::RIGHT,
                                       ttb::ui::Center::VAlignment::BOTTOM };
        auto testElement = TestElement{ framework, true, 0.5f, true, 0.5f };

        center.child( &testElement );
        root.child( &center );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.size() );
        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.localToScreen( { 0.0f, 0.0f } ) );
        REQUIRE( ttb::Vector{ 1.0f, 1.0f } == center.size() );
    }

    SECTION( "Left" )
    {
        auto root = ttb::ui::Root{ framework, { 1.0f, 1.0f } };
        auto center = ttb::ui::Center{ framework,
                                       ttb::ui::Center::HAlignment::LEFT,
                                       ttb::ui::Center::VAlignment::TOP };
        auto testElement = TestElement{ framework, true, 0.5f, true, 0.5f };

        center.child( &testElement );
        root.child( &center );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == testElement.localToScreen( { 0.0f, 0.0f } ) );
        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == center.size() );
    }

    SECTION( "Min size" )
    {
        auto root = ttb::ui::Root{ framework, { 0.0f, 0.0f } };
        auto center = ttb::ui::Center{ framework,
                                       ttb::ui::Center::HAlignment::LEFT,
                                       ttb::ui::Center::VAlignment::TOP };
        auto testElement = TestElement{ framework, true, 0.5f, true, 0.5f };

        center.child( &testElement );
        root.child( &center );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == testElement.size() );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == testElement.localToScreen( { 0.0f, 0.0f } ) );
        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == center.size() );
    }
}
