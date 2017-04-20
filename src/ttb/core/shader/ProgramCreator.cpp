#include "ProgramCreator.hpp"

#include <ttb/core/shader/Program.hpp>
#include <ttb/core/shader/Shader.hpp>

namespace ttb
{
    ProgramCreator::ProgramCreator()
    {
    }

    ProgramCreator& ProgramCreator::attachShader( std::unique_ptr< Shader > shader )
    {
        m_shaders.push_back( std::move( shader ) );
    }

    std::unique_ptr< Program > ProgramCreator::finish()
    {
        return std::unique_ptr< Program >( new Program( std::move( m_shaders ) ) );
    }
}
