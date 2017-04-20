#pragma once

#include <memory>
#include <vector>

// forward declarations
//=================================================================================================

namespace ttb
{
    class Shader;
    class Program;
}



// declarations
//=================================================================================================

namespace ttb
{
    class ProgramCreator
    {
    public:
        ProgramCreator& attachShader( std::unique_ptr< Shader > shader );
        std::unique_ptr< Program > finish();

    private:
        ProgramCreator();

        std::vector< std::unique_ptr< Shader > > m_shaders;

        friend class Program;
    };
}
