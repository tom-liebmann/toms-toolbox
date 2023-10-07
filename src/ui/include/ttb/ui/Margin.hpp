#pragma once

#include <ttb/ui/xml_loading.hpp>

#include <regex>


namespace ttb::ui
{
    class Margin
    {
    public:
        static Margin parse( XmlNode const& node, XmlLoader& loader );

        constexpr Margin();
        constexpr Margin( float value );
        constexpr Margin( float rightLeft, float topBottom );
        constexpr Margin( float right, float top, float left, float bottom );

        constexpr float getRightLeft() const;
        constexpr float getTopBottom() const;

        constexpr float getRight() const;
        constexpr float getTop() const;
        constexpr float getLeft() const;
        constexpr float getBottom() const;

        constexpr Margin setRight( float value ) const;
        constexpr Margin setTop( float value ) const;
        constexpr Margin setLeft( float value ) const;
        constexpr Margin setBottom( float value ) const;

    private:
        static Margin parse( std::string_view const& value );

        float m_right{ 0.0f };
        float m_top{ 0.0f };
        float m_left{ 0.0f };
        float m_bottom{ 0.0f };
    };
}


namespace ttb::ui
{
    inline Margin Margin::parse( XmlNode const& node, XmlLoader& loader )
    {
        if( auto const value = loader.attrValue( node, "margin" ) )
        {
            return parse( value.value() );
        }

        return {};
    }

    inline constexpr Margin::Margin() = default;

    inline constexpr Margin::Margin( float value )
        : m_right{ value }, m_top{ value }, m_left{ value }, m_bottom{ value }
    {
    }

    inline constexpr Margin::Margin( float rightLeft, float topBottom )
        : m_right{ rightLeft }, m_top{ topBottom }, m_left{ rightLeft }, m_bottom{ topBottom }
    {
    }

    inline constexpr Margin::Margin( float right, float top, float left, float bottom )
        : m_right{ right }, m_top{ top }, m_left{ left }, m_bottom{ bottom }
    {
    }

    inline constexpr float Margin::getRightLeft() const
    {
        return m_right + m_left;
    }

    inline constexpr float Margin::getTopBottom() const
    {
    return m_top + m_bottom;
    }

    inline constexpr float Margin::getRight() const
    {
        return m_right;
    }

    inline constexpr float Margin::getTop() const
    {
        return m_top;
    }

    inline constexpr float Margin::getLeft() const
    {
        return m_left;
    }

    inline constexpr float Margin::getBottom() const
    {
        return m_bottom;
    }

    inline constexpr Margin Margin::setRight( float value ) const
    {
        return { value, m_top, m_left, m_bottom };
    }

    inline constexpr Margin Margin::setTop( float value ) const
    {
        return { m_right, value, m_left, m_bottom };
    }

    inline constexpr Margin Margin::setLeft( float value ) const
    {
        return { m_right, m_top, value, m_bottom };
    }

    inline constexpr Margin Margin::setBottom( float value ) const
    {
        return { m_right, m_top, m_left, value };
    }

    inline Margin Margin::parse( std::string_view const& value )
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
            return { std::stof( match[ 1 ] ),
                     std::stof( match[ 2 ] ),
                     std::stof( match[ 3 ] ),
                     std::stof( match[ 4 ] ) };
        }

        if( std::regex_match( std::begin( value ), std::end( value ), match, PATTERN_2 ) )
        {
            return { std::stof( match[ 1 ] ), std::stof( match[ 2 ] ) };
        }

        if( std::regex_match( std::begin( value ), std::end( value ), match, PATTERN_1 ) )
        {
            return { std::stof( match[ 1 ] ) };
        }

        throw std::runtime_error( "Invalid margin value" );
    }
}
