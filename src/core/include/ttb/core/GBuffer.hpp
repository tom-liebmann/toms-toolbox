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
        class Handle;

        static Modifier create();

        ~GBuffer();

        std::shared_ptr< Texture2D > const& drawBuffer( uint8_t unit ) const;

        std::shared_ptr< Texture2D > const& depthBuffer() const;

        /// @copydoc RenderTarget::viewport() const
        virtual Viewport viewport() const override;

    private:
        GBuffer();

        /// @copydoc RenderTarget::begin( State::Data& ) const
        virtual void begin( State::Data& data ) const override;

        /// @copydoc RenderTarget::begin( State::Data& ) const
        virtual void suspend( State::Data& data ) const override;

        /// @copydoc RenderTarget::begin( State::Data& ) const
        virtual void resume( State::Data& data ) const override;

        /// @copydoc RenderTarget::end( State::Data& ) const
        virtual void end( State::Data& data ) const override;

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
