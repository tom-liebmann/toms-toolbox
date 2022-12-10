#include <ttb/ui/elements/Margin.hpp>

#include <ttb/core/uniform.hpp>
#include <ttb/math/matrix_operations.hpp>
#include <ttb/ui/XmlFactory.hpp>

#include <regex>


namespace ttb::ui
{
    namespace
    {
        auto const factory = XmlFactory< Margin >{ "margin" };

        void parseMargins(
            std::string_view const& value, float& right, float& top, float& left, float& bottom );
    }
}


namespace ttb::ui
{
    Margin::Margin( Root& root, float right, float top, float left, float bottom )
        : WrappedElement{ root }, m_right{ right }, m_top{ top }, m_left{ left }, m_bottom{ bottom }
    {
    }

    Margin::Margin( Root& root, float hMargin, float vMargin )
        : Margin{ root, hMargin, vMargin, hMargin, vMargin }
    {
    }

    Margin::Margin( Root& root, float margin ) : Margin{ root, margin, margin, margin, margin }
    {
    }

    Margin::Margin( Root& root, rapidxml::xml_node<> const& node, XmlLoader& loader )
        : WrappedElement{ root }
    {
        if( auto const value = loader.attrValue( node, "value" ); value )
        {
            parseMargins( *value, m_right, m_top, m_left, m_bottom );
        }

        if( auto child = node.first_node(); child )
        {
            wrappedChild( loader.loadElement( root, *child ) );
        }
    }

    void Margin::child( Element* element )
    {
        wrappedChild( element );
    }

    void Margin::right( float value, bool isLastChange )
    {
        m_right = value;

        if( isLastChange )
        {
            changed();
        }
    }

    void Margin::top( float value, bool isLastChange )
    {
        m_top = value;

        if( isLastChange )
        {
            changed();
        }
    }

    void Margin::left( float value, bool isLastChange )
    {
        m_left = value;

        if( isLastChange )
        {
            changed();
        }
    }

    void Margin::bottom( float value, bool isLastChange )
    {
        m_bottom = value;

        if( isLastChange )
        {
            changed();
        }
    }

    auto Margin::fit( Size const& size ) -> Size
    {
        if( auto const child = wrappedChild(); child )
        {
            auto const childSize =
                child->fit( { size( 0 ) - m_left - m_right, size( 1 ) - m_top - m_bottom } );

            return { childSize( 0 ) + m_left + m_right, childSize( 1 ) + m_top + m_bottom };
        }

        return size;
    }

    void Margin::offset( Offset const& value )
    {
        Element::offset( value );

        if( auto const child = wrappedChild(); child )
        {
            child->offset( { value( 0 ) + m_left, value( 1 ) + m_top } );
        }
    }

    void Margin::size( Size const& value )
    {
        Element::size( value );

        if( auto const child = wrappedChild(); child )
        {
            child->size( { value( 0 ) - m_left - m_right, value( 1 ) - m_top - m_bottom } );
        }
    }
}


namespace ttb::ui
{
    namespace
    {
        void parseMargins(
            std::string_view const& value, float& right, float& top, float& left, float& bottom )
        {
            static auto const PATTERN_4 = std::regex{
                R"pat(\s*([\d\-\+\.eE]+)\s+([\d\-\+\.eE]+)\s+([\d\-\+\.eE]+)\s+([\d\-\+\.eE]+)\s*)pat"
            };

            static auto const PATTERN_2 =
                std::regex{ R"pat(\s*([\d\-\+\.eE]+)\s+([\d\-\+\.eE]+)\s*)pat" };

            static auto const PATTERN_1 = std::regex{ R"pat(\s*([\d\-\+\.eE]+)\s*)pat" };

            using svmatch = std::match_results< std::string_view::const_iterator >;

            auto match = svmatch{};

            if( std::regex_match( std::begin( value ), std::end( value ), match, PATTERN_4 ) )
            {
                right = std::stof( match[ 1 ] );
                top = std::stof( match[ 2 ] );
                left = std::stof( match[ 3 ] );
                bottom = std::stof( match[ 4 ] );
                return;
            }

            if( std::regex_match( std::begin( value ), std::end( value ), match, PATTERN_2 ) )
            {
                right = std::stof( match[ 1 ] );
                left = right;
                top = std::stof( match[ 2 ] );
                bottom = top;
                return;
            }

            if( std::regex_match( std::begin( value ), std::end( value ), match, PATTERN_1 ) )
            {
                right = std::stof( match[ 1 ] );
                top = right;
                left = right;
                bottom = right;
                return;
            }

            throw std::runtime_error( "Invalid margin value" );
        }
    }
}
