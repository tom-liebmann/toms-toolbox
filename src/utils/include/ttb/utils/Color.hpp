#pragma once

#include <ttb/math/utils.hpp>

#include <cmath>


namespace ttb
{
    class ColorRgb;
    class ColorHsl;
}


namespace ttb
{
    class ColorRgb
    {
    public:
        static constexpr ColorRgb createF( float r, float g, float b );

        static constexpr ColorRgb createI( uint8_t r, uint8_t g, uint8_t b );

        constexpr ColorRgb();

        constexpr explicit ColorRgb( ColorHsl const& rhs );

        constexpr float rF() const;

        constexpr float gF() const;

        constexpr float bF() const;

        constexpr uint8_t rI() const;

        constexpr uint8_t gI() const;

        constexpr uint8_t bI() const;

    private:
        constexpr ColorRgb( uint8_t r, uint8_t g, uint8_t b );

        uint8_t m_r{ 0 };
        uint8_t m_g{ 0 };
        uint8_t m_b{ 0 };
    };

    std::ostream& operator<<( std::ostream& stream, ColorRgb const& color );


    class ColorHsl
    {
    public:
        constexpr ColorHsl();

        constexpr ColorHsl( float h, float s, float l );

        constexpr explicit ColorHsl( ColorRgb const& rhs );

        constexpr float h() const;

        constexpr float s() const;

        constexpr float l() const;

        constexpr ColorHsl lighten( float value ) const;

    private:
        float m_h{ 0.0f };
        float m_s{ 0.0f };
        float m_l{ 0.0f };
    };

    std::ostream& operator<<( std::ostream& stream, ColorHsl const& color );


    inline constexpr ColorRgb ColorRgb::createF( float r, float g, float b )
    {
        return { static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, r * 255.0f ) ) ),
                 static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, g * 255.0f ) ) ),
                 static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, b * 255.0f ) ) ) };
    }

    inline constexpr ColorRgb ColorRgb::createI( uint8_t r, uint8_t g, uint8_t b )
    {
        return { r, g, b };
    }

    inline constexpr ColorRgb::ColorRgb() = default;

    inline constexpr ColorRgb::ColorRgb( ColorHsl const& rhs )
    {
        auto const c = ( 1.0f - ttb::abs( 2.0f * rhs.l() - 1.0f ) ) * rhs.s();
        auto const x = c * ( 1.0f - ttb::abs( ttb::fmod( rhs.h() / 60.0f, 2.0f ) - 1.0f ) );
        auto const m = rhs.l() - c / 2.0f;

        if( rhs.h() <= 60.0f )
        {
            m_r =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( c + m ) ) ) );
            m_g =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( x + m ) ) ) );
            m_b = static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * m ) ) );
        }
        else if( rhs.h() <= 120.0f )
        {
            m_r =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( x + m ) ) ) );
            m_g =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( c + m ) ) ) );
            m_b = static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * m ) ) );
        }
        else if( rhs.h() <= 180.0f )
        {
            m_r = static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * m ) ) );
            m_g =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( c + m ) ) ) );
            m_b =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( x + m ) ) ) );
        }
        else if( rhs.h() <= 240.0f )
        {
            m_r = static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * m ) ) );
            m_g =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( x + m ) ) ) );
            m_b =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( c + m ) ) ) );
        }
        else if( rhs.h() <= 300.0f )
        {
            m_r =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( x + m ) ) ) );
            m_g = static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * m ) ) );
            m_b =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( c + m ) ) ) );
        }
        else
        {
            m_r =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( c + m ) ) ) );
            m_g = static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * m ) ) );
            m_b =
                static_cast< uint8_t >( std::min( 255.0f, std::max( 0.0f, 255.0f * ( x + m ) ) ) );
        }
    }

    inline constexpr float ColorRgb::rF() const
    {
        return m_r / 255.0f;
    }

    inline constexpr float ColorRgb::gF() const
    {
        return m_g / 255.0f;
    }

    inline constexpr float ColorRgb::bF() const
    {
        return m_b / 255.0f;
    }

    inline constexpr uint8_t ColorRgb::rI() const
    {
        return m_r;
    }

    inline constexpr uint8_t ColorRgb::gI() const
    {
        return m_g;
    }

    inline constexpr uint8_t ColorRgb::bI() const
    {
        return m_b;
    }

    inline constexpr ColorRgb::ColorRgb( uint8_t r, uint8_t g, uint8_t b )
        : m_r{ r }, m_g{ g }, m_b{ b }
    {
    }


    inline constexpr ColorHsl::ColorHsl() = default;

    inline constexpr ColorHsl::ColorHsl( float h, float s, float l ) : m_h{ h }, m_s{ s }, m_l{ l }
    {
    }

    inline constexpr ColorHsl::ColorHsl( ColorRgb const& rhs )
    {
        auto const cmax = std::max( rhs.rF(), std::max( rhs.gF(), rhs.bF() ) );
        auto const cmin = std::min( rhs.rF(), std::min( rhs.gF(), rhs.bF() ) );
        auto const delta = cmax - cmin;

        m_l = ( cmax + cmin ) * 0.5f;

        if( ttb::abs( delta ) < 1e-5 )
        {
            m_h = 0.0f;
            m_s = 0.0f;
        }
        else
        {
            if( m_l < 0.5f )
            {
                m_s = delta / ( cmax + cmin );
            }
            else
            {
                m_s = delta / ( 2.0f - cmax - cmin );
            }

            if( cmax == rhs.rF() )
            {
                m_h = 60.0f * ( rhs.gF() - rhs.bF() ) / ( cmax - cmin );
            }
            else if( cmax == rhs.gF() )
            {
                m_h = 60.0f * ( rhs.bF() - rhs.rF() ) / ( cmax - cmin ) + 120.0f;
            }
            else
            {
                m_h = 60.0f * ( rhs.rF() - rhs.gF() ) / ( cmax - cmin ) + 240.0f;
            }

            if( m_h < 0.0f )
            {
                m_h += 360.0f;
            }
        }
    }

    inline constexpr float ColorHsl::h() const
    {
        return m_h;
    }

    inline constexpr float ColorHsl::s() const
    {
        return m_s;
    }

    inline constexpr float ColorHsl::l() const
    {
        return m_l;
    }

    inline constexpr ColorHsl ColorHsl::lighten( float value ) const
    {
        return { m_h, m_s, ttb::clamp( m_l + value, 0.0f, 1.0f ) };
    }


    inline std::ostream& operator<<( std::ostream& stream, ColorRgb const& color )
    {
        return stream << "rgb(" << uint32_t{ color.rI() } << ", " << uint32_t{ color.gI() } << ", "
                      << uint32_t{ color.bI() } << ')';
    }


    inline std::ostream& operator<<( std::ostream& stream, ColorHsl const& color )
    {
        return stream << "hsl(" << color.h() << ", " << color.s() << ", " << color.l() << ')';
    }
}
