#pragma once

#include <memory>
#include <list>

namespace tgMath
{
    class Matrix4f;
}

namespace tgCore
{
    class ShaderProgram;

    class State
    {
        public:
            static State& getInstance();

            ~State();

            // projection
            void setProjectionMatrix( const tgMath::Matrix4f& matrix );

            // modelview
            void pushMatrix();
            void popMatrix();
            void setMatrix( const tgMath::Matrix4f& matrix );
            void applyMatrix( const tgMath::Matrix4f& matrix );

            // shader
            void pushShader( std::shared_ptr< tgCore::ShaderProgram > shader );
            void popShader();

            void apply();

        private:
            State();

            // projection
            tgMath::Matrix4f* m_projectionMatrix;
            bool m_projectionMatrixSet;

            // modelview
            std::list< tgMath::Matrix4f* > m_modelViewMatrixStack;
            bool m_modelViewMatrixSet;

            // shader
            std::list< std::shared_ptr< tgCore::ShaderProgram > > m_shaderStack;
    };



    inline State& State::getInstance()
    {
        static State state;
        return state;
    }
}
