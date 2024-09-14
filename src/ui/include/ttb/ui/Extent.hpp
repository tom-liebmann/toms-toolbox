#pragma once


namespace ttb::ui
{
    class Extent
    {
    public:
        enum class Type
        {
            UNSPECIFIED,
            FIXED,
            MATCH_PARENT,
            MATCH_CHILD,
        };

        constexpr Extent();

        constexpr Extent( Type type );

        constexpr Extent( float value );

        constexpr Type getType() const;

        constexpr float getValue() const;

    private:
        Type m_type;
        float m_value{ 0.0f };
    };


    constexpr Extent::Extent() : m_type{ Type::UNSPECIFIED }
    {
    }

    constexpr Extent::Extent( Type type ) : m_type{ type }
    {
    }

    constexpr Extent::Extent( float value ) : m_type{ Type::FIXED }, m_value{ value }
    {
    }

    constexpr auto Extent::getType() const -> Type
    {
        return m_type;
    }

    constexpr float Extent::getValue() const
    {
        return m_value;
    }
}
