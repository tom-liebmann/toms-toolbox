#include <catch2/catch.hpp>

#include <ttb/core/resources/Manager.hpp>
#include <ttb/ui/Root.hpp>
#include <ttb/ui/elements/Group.hpp>
// #include <ttb/ui/elements/Center.hpp>
// #include <ttb/ui/elements/Flex.hpp>
// #include <ttb/ui/elements/Margin.hpp>
// #include <ttb/ui/elements/ScrollArea.hpp>
#include <ttb/utils/EventManager.hpp>

#include <fmt/core.h>


namespace
{
    class TestElement : public ttb::ui::Element
    {
    public:
        TestElement( ttb::ui::Root& root ) : Element{ root }
        {
        }

        virtual void render( ttb::State& state ) const override
        {
        }
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

        ttb::Vector< TType, TDim > const& getValue() const
        {
            return m_value;
        }

    private:
        TType m_epsilon{ 1e-5 };
        ttb::Vector< TType, TDim > m_value;
    };

    template < typename TType, std::size_t TDim >
    Approx( ttb::Vector< TType, TDim > const& ) -> Approx< ttb::Vector< TType, TDim > >;


    template < typename TType, std::size_t TDim >
    std::ostream& operator<<( std::ostream& stream,
                              ttb::Approx< ttb::Vector< TType, TDim > > const& vec )
    {
        return stream << vec.getValue();
    }
}


TEST_CASE( "Element", "[ui][elements]" )
{
    using namespace ttb::ui;

    auto resourceManager = std::shared_ptr< ttb::resources::Manager >();
    auto root = ttb::ui::Root{ resourceManager, { 1.0f, 1.0f } };

    SECTION( "Unspecified width" )
    {
        auto ele = TestElement{ root };
        ele.setHeight( 0.5f );

        REQUIRE_THROWS( root.setChild( &ele ) );
    }

    SECTION( "Unspecified Height" )
    {
        auto ele = TestElement{ root };
        ele.setWidth( 0.5f );

        REQUIRE_THROWS( root.setChild( &ele ) );
    }

    SECTION( "Fixed size" )
    {
        auto ele = TestElement{ root };
        ele.setWidth( 0.5f );
        ele.setHeight( 0.5f );

        root.setChild( &ele );

        REQUIRE( ttb::Vector{ 0.5f, 0.5f } == ttb::Approx{ ele.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ ele.getPosition() } );
    }

    SECTION( "Match parent" )
    {
        auto ele = TestElement{ root };
        ele.setWidth( Extent::Type::MATCH_PARENT );
        ele.setHeight( Extent::Type::MATCH_PARENT );

        root.setChild( &ele );

        REQUIRE( ttb::Vector{ 1.0f, 1.0f } == ttb::Approx{ ele.getSize() } );
        REQUIRE( ttb::Vector{ 0.0f, 0.0f } == ttb::Approx{ ele.getPosition() } );
    }

    SECTION( "Padding" )
    {
        auto ele = TestElement{ root };

        ele.setWidth( Extent::Type::MATCH_PARENT );
        ele.setHeight( Extent::Type::MATCH_PARENT );

        ele.setLeft( 0.1f );
        ele.setRight( 0.2f );
        ele.setTop( 0.3f );
        ele.setBottom( 0.1f );

        root.setChild( &ele );

        REQUIRE( ttb::Vector{ 0.7f, 0.6f } == ttb::Approx{ ele.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.3f } == ttb::Approx{ ele.getPosition() } );
    }
}

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

        auto child = TestElement{ root };
        child.setWidth( 0.5f );
        child.setHeight( 0.4f );

        group.add( &child );
        root.setChild( &group );

        REQUIRE( ttb::Vector{ 0.5f, 0.4f } == ttb::Approx{ group.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.2f } == ttb::Approx{ group.getPosition() } );

        REQUIRE( ttb::Vector{ 0.5f, 0.4f } == ttb::Approx{ child.getSize() } );
        REQUIRE( ttb::Vector{ 0.1f, 0.2f } == ttb::Approx{ child.getPosition() } );
    }
}
