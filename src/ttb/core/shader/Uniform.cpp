#include "Uniform.hpp"

namespace ttb
{
    UniformBase::UniformBase( std::string const& name )
        : m_name( name )
    {
    }

    UniformBase::~UniformBase()
    {
    }

    std::string const& UniformBase::name() const
    {
        return m_name;
    }
}
