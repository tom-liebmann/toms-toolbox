#pragma once

#include <string>


namespace ttb::resources
{
    std::string dirname( std::string const& filename );

    std::string loadFile( std::string const& filename );
}
