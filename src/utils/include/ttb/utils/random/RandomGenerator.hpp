#pragma once

#include <concepts>
#include <random>
#include <type_traits>


namespace ttb
{
    class RandomGenerator
    {
    public:
        using Generator = std::mt19937;

        RandomGenerator();

        Generator& getGenerator();

        // template < std::integral TType >
        // TType getUniform( TType min, TType max );

        // template < std::floating_point TType >
        // TType getUniform( TType min, TType max );

        template < typename TType >
        TType getUniform( TType min, TType max );

        template < typename TType >
        TType getNormal( TType mean, TType stddev );

        bool getBool( double probability );

    private:
        Generator m_generator;
    };
}


namespace ttb
{
    inline auto RandomGenerator::getGenerator() -> Generator&
    {
        return m_generator;
    }

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

    template < typename TType >
    inline TType RandomGenerator::getNormal( TType mean, TType stddev )
    {
        auto dist = std::normal_distribution< TType >{ mean, stddev };
        return dist( m_generator );
    }

    inline bool RandomGenerator::getBool( double probability )
    {
        auto dist = std::bernoulli_distribution{ probability };
        return dist( m_generator );
    }
}
