#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <tgCore/Viewport.hpp>

#include <memory>
#include <list>

// forward declarations
//=============================================================================

namespace tg
{
    class Window;
    class ShaderProgram;
}

namespace tgMath
{
    class Matrix4f;
}



// declarations
//=============================================================================

namespace tg
{
    class State
    {
    public:
        State( Window& window );
        ~State();

        // projection
        void setProjectionMatrix( const tgMath::Matrix4f& matrix );

        // modelview
        void pushMatrix();
        void popMatrix();
        void setMatrix( const tgMath::Matrix4f& matrix );
        void applyMatrix( const tgMath::Matrix4f& matrix );

        // shader
        void pushShader( std::shared_ptr< ShaderProgram > shader );
        void popShader();

        // viewport
        void pushViewport( Viewport viewport );
        void popViewport();

        // events
        void onWindowResize();
        void apply();

    private:
        Window& m_window;

        // projection
        std::unique_ptr< tgMath::Matrix4f > m_projectionMatrix;
        bool m_projectionMatrixSet;

        // modelview
        std::list< std::unique_ptr< tgMath::Matrix4f > > m_modelViewMatrixStack;
        bool m_modelViewMatrixSet;

        // shader
        std::list< std::shared_ptr< ShaderProgram > > m_shaderStack;

        // viewport
        std::unique_ptr< Viewport > m_windowViewport;
        std::list< Viewport > m_viewportStack;
    };



}



// definitions
//=============================================================================

namespace tg
{
}
