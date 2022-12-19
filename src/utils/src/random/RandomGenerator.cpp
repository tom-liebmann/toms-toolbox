#include <ttb/utils/random/RandomGenerator.hpp>


namespace ttb
{
    RandomGenerator::RandomGenerator() : m_generator{ std::random_device{}() }
    {
    }
}
