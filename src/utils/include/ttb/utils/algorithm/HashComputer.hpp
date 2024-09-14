#pragma once

#include <functional>


namespace ttb
{
    class HashComputer
    {
    public:
        template < typename TType >
        void push( TType value );

        std::size_t getValue() const;

    private:
        std::size_t m_value{ 0 };
    };
}

template < typename TType >
struct std::hash< std::vector< TType > >
{
    std::size_t operator()( std::vector< TType > const& values ) const noexcept
    {
        auto hashComputer = ttb::HashComputer{};
        for( auto const& value : values )
        {
            hashComputer.push( value );
        }
        return hashComputer.getValue();
    }
};

template < typename TType >
std::size_t computeHash( TType const& value );


namespace ttb
{
    template < typename TType >
    inline void HashComputer::push( TType value )
    {
        m_value ^= std::hash< TType >{}( value ) + ( m_value << 6 ) + ( m_value >> 2 );
    }

    inline std::size_t HashComputer::getValue() const
    {
        return m_value;
    }
}

template < typename TType >
std::size_t computeHash( TType const& value )
{
    return std::hash< TType >{}( value );
}
