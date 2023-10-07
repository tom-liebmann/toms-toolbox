#pragma once


namespace ttb::ui
{
    class FitExtent
    {
    public:
        enum class Type
        {
            MATCH_PARENT,
            FIXED,
        };

        constexpr FitExtent( Type type );

        constexpr FitExtent( float value );

        constexpr Type getType() const;

        constexpr float getValue() const;

    private:
        Type m_type;
        float m_value;
    };
}


namespace ttb::ui
{
    inline constexpr FitExtent::FitExtent( Type type ) : m_type{ type }
    {
    }

    inline constexpr FitExtent::FitExtent( float value ) : m_type{ Type::FIXED }, m_value{ value }
    {
    }

    inline constexpr auto FitExtent::getType() const -> Type
    {
        return m_type;
    }

    inline constexpr float FitExtent::getValue() const
    {
        return m_value;
    }
}
