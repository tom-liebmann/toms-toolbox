#pragma once

#include <ttb/core/Viewport.hpp>
#include <ttb/math/Matrix.hpp>

#include <stack>
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

        // geometry
        void pushArrayObject( GLuint arrayObject );
        void popArrayObject();

        // viewport
        void pushViewport( Viewport const& viewport );
        void popViewport();

        // events
        void apply();

    private:
        // render target
        std::stack< std::shared_ptr< ttb::RenderTarget > > m_renderTargetStack;

        // projection
        ttb::Matrix< float, 4, 4 > m_projectionMatrix;
        bool m_projectionMatrixSet;

        // modelview
        std::stack< ttb::Matrix< float, 4, 4 > > m_modelViewMatrixStack;
        bool m_modelViewMatrixSet;

        // shader
        std::stack< std::shared_ptr< Program > > m_programStack;

        // geometry
        std::stack< GLuint > m_arrayObjectStack;

        // viewport
        std::stack< Viewport > m_viewportStack;
    };
}
