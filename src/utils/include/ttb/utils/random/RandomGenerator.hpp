#pragma once

#include <concepts>
#include <random>
#include <type_traits>


namespace ttb
{
    class RandomGenerator
    {
    public:
        RandomGenerator();

        // template < std::integral TType >
        // TType getUniform( TType min, TType max );

        // template < std::floating_point TType >
        // TType getUniform( TType min, TType max );

        template < typename TType >
        TType getUniform( TType min, TType max );

        bool getBool( double probability );

    private:
        std::mt19937 m_generator;
    };
}


namespace ttb
{
    // template < std::integral TType >
    // inline TType RandomGenerator::getUniform( TType min, TType max )
    // {
    //     auto dist = std::uniform_int_distribution< TType >{ min, max };
    //     return dist( m_generator );
    // }

    // template < std::floating_point TType >
    // inline TType RandomGenerator::getUniform( TType min, TType max )
    // {
    //     auto dist = std::uniform_real_distribution< TType >{ min, max };
    //     return dist( m_generator );
    // }

    template < typename TType >
    TType RandomGenerator::getUniform( TType min, TType max )
    {
        if constexpr( std::is_integral_v< TType > )
        {
            auto dist = std::uniform_int_distribution< TType >{ min, max };
            return dist( m_generator );
        }
        else
        {
            auto dist = std::uniform_real_distribution< TType >{ min, max };
            return dist( m_generator );
        }
    }

    inline bool RandomGenerator::getBool( double probability )
    {
        auto dist = std::bernoulli_distribution{ probability };
        return dist( m_generator );
    }
}
