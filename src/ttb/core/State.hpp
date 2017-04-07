#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <ttb/core/Viewport.hpp>
#include <ttb/math/Matrix4f.hpp>

#include <list>
#include <memory>

// forward declarations
//=============================================================================

namespace ttb
{
    class Window;
    class ShaderProgram;
}



// declarations
//=============================================================================

namespace ttb
{
    class State
    {
    public:
        State( Window& window );
        ~State();

        // projection
        void setProjectionMatrix( const ttb::Matrix4f& matrix );
        ttb::Matrix4f const& getProjectionMatrix() const;

        // modelview
        void pushMatrix();
        void popMatrix();
        void setMatrix( const ttb::Matrix4f& matrix );
        void applyMatrix( const ttb::Matrix4f& matrix );

        ttb::Matrix4f const& getModelViewMatrix() const;

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
        std::unique_ptr< ttb::Matrix4f > m_projectionMatrix;
        bool m_projectionMatrixSet;

        // modelview
        std::list< std::unique_ptr< ttb::Matrix4f > > m_modelViewMatrixStack;
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

namespace ttb
{
}
