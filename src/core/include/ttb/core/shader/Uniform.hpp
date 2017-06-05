#pragma once

#include <GL/gl.h>

#include <string>

namespace ttb
{
    class UniformBase
    {
    public:
        UniformBase( std::string const& name );

        virtual ~UniformBase();

        std::string const& name() const;

        virtual void apply( GLint location ) const = 0;

    private:
        std::string m_name;
    };
}
