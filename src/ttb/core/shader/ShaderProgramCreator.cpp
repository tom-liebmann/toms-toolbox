#include "ShaderProgramCreator.hpp"

#include <ttb/core/shader/ShaderProgram.hpp>
#include <ttb/core/shader/Shader.hpp>

namespace ttb
{
    ShaderProgramCreator::ShaderProgramCreator()
    {
    }

    ShaderProgramCreator& ShaderProgramCreator::attachShader( std::unique_ptr< Shader > shader )
    {
        m_shaders.push_back( std::move( shader ) );
    }

    std::unique_ptr< ShaderProgram > ShaderProgramCreator::finish()
    {
        return std::unique_ptr< ShaderProgram >( new ShaderProgram( std::move( m_shaders ) ) );
    }
}
