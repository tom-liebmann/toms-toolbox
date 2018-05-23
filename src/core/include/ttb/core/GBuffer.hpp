#pragma once

#include "RenderTarget.hpp"

#include <ttb/core/gl.hpp>

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
        class Modifier;

        static Modifier create();

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
    };


    class GBuffer::Modifier
    {
    public:
        Modifier& drawBuffer( uint8_t unit, std::shared_ptr< Texture2D > buffer );

        Modifier& depthBuffer( std::shared_ptr< Texture2D > buffer );

        std::shared_ptr< GBuffer > finish();

    private:
        Modifier( std::shared_ptr< GBuffer > buffer );

        std::shared_ptr< GBuffer > m_buffer;

        friend Modifier modify( std::shared_ptr< GBuffer > buffer );
    };


    GBuffer::Modifier modify( std::shared_ptr< GBuffer > buffer );
}
