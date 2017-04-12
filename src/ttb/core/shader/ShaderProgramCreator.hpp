#pragma once

#include <memory>
#include <vector>

// forward declarations
//=================================================================================================

namespace ttb
{
    class Shader;
    class ShaderProgram;
}



// declarations
//=================================================================================================

namespace ttb
{
    class ShaderProgramCreator
    {
    public:
        ShaderProgramCreator& attachShader( std::unique_ptr< Shader > shader );
        std::unique_ptr< ShaderProgram > finish();

    private:
        ShaderProgramCreator();

        std::vector< std::unique_ptr< Shader > > m_shaders;

        friend class ShaderProgram;
    };
}
