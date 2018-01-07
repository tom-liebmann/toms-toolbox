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
    class GBuffer;


    class GBufferModifier
    {
    public:
        GBufferModifier( State& state, std::shared_ptr< GBuffer > buffer );

        GBufferModifier& drawBuffer( uint8_t unit, std::shared_ptr< Texture2D > buffer );

        GBufferModifier& depthBuffer( std::shared_ptr< Texture2D > buffer );

        std::shared_ptr< GBuffer > finish();

    private:
        State& m_state;
        std::shared_ptr< GBuffer > m_buffer;
    };


    class GBuffer : public RenderTarget
    {
    public:
        static GBufferModifier create( ttb::State& state );

        ~GBuffer();

        std::shared_ptr< Texture2D > const& drawBuffer( uint8_t unit ) const;

        std::shared_ptr< Texture2D > const& depthBuffer() const;

        // Override: RenderTarget
        virtual size_t width() const override;
        virtual size_t height() const override;
        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;

    private:
        GBuffer();

        GLuint m_frameBufferObject;

        std::vector< std::shared_ptr< Texture2D > > m_drawBuffers;
        std::vector< GLenum > m_drawBufferIDs;
        std::shared_ptr< Texture2D > m_depthBuffer;

        friend class GBufferModifier;
    };
}
