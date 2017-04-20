#pragma once

#include "RenderTarget.hpp"

#include <GL/glew.h>

#include <GL/gl.h>

#include <cstdint>
#include <memory>
#include <vector>

// forward declarations
//=============================================================================

namespace ttb
{
    class Texture2D;
    class State;
}



// declarations
//=============================================================================

namespace ttb
{
    class GBuffer : public RenderTarget
    {
    public:
        GBuffer();

        ~GBuffer();

        uint16_t getWidth() const;
        uint16_t getHeight() const;
        const std::shared_ptr< Texture2D >& getDrawBuffer( int8_t unit ) const;
        const std::shared_ptr< Texture2D >& getDepthBuffer() const;

        void setDrawBuffer( uint8_t unit, std::shared_ptr< Texture2D > buffer );
        void setDepthBuffer( std::shared_ptr< Texture2D > buffer );

        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;

    private:
        GLuint m_frameBufferObject;

        std::vector< std::shared_ptr< Texture2D > > m_drawBuffers;
        std::vector< GLenum > m_drawBufferIDs;
        std::shared_ptr< Texture2D > m_depthBuffer;
    };
}
