#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace tgCore
{
    class Texture2D;

    class GBuffer
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

            void begin() const;
            void end() const;

        private:
            GLuint m_frameBufferObject;            

            std::vector< std::shared_ptr< Texture2D > > m_drawBuffers;
            std::vector< GLenum > m_drawBufferIDs;
            std::shared_ptr< Texture2D > m_depthBuffer;
    };
}
