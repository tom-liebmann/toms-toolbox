#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <ttb/core/Viewport.hpp>
#include <ttb/math/Matrix.hpp>

#include <list>
#include <memory>

// forward declarations
//=============================================================================

namespace ttb
{
    class Program;
    class RenderTarget;
}



// declarations
//=============================================================================

namespace ttb
{
    class State
    {
    public:
        State();
        ~State();

        // render target
        void pushTarget( std::shared_ptr< RenderTarget > const& target );
        void popTarget();

        RenderTarget const& renderTarget() const;

        // projection
        void projectionMatrix( ttb::Matrix< float, 4, 4 > const& matrix );
        ttb::Matrix< float, 4, 4 > const& projectionMatrix() const;

        // modelview
        void pushMatrix();
        void popMatrix();
        void setMatrix( ttb::Matrix< float, 4, 4 > const& matrix );
        void applyMatrix( ttb::Matrix< float, 4, 4 > const& matrix );

        ttb::Matrix< float, 4, 4 > const& modelViewMatrix() const;

        // shader
        void pushProgram( std::shared_ptr< Program > const& program );
        void popProgram();

        Program& program();

        // viewport
        void pushViewport( Viewport const& viewport );
        void popViewport();

        // events
        void apply();

    private:
        // render target
        std::list< std::shared_ptr< ttb::RenderTarget > > m_renderTargetStack;

        // projection
        ttb::Matrix< float, 4, 4 > m_projectionMatrix;
        bool m_projectionMatrixSet;

        // modelview
        std::list< ttb::Matrix< float, 4, 4 > > m_modelViewMatrixStack;
        bool m_modelViewMatrixSet;

        // shader
        std::list< std::shared_ptr< Program > > m_programStack;

        // viewport
        std::list< Viewport > m_viewportStack;
    };
}
