#pragma once

#include <ttb/core/gl.hpp>
#include <ttb/core/texture/Texture.hpp>

#include <cstdint>
#include <memory>
#include <vector>


// declarations
//=============================================================================

namespace ttb
{
    class Texture2D : public Texture
    {
    public:
        class Modifier;
        class ConstModifier;

        static Modifier create( size_t width,
                                size_t height,
                                GLint internalFormat,
                                uint8_t levels = 1,
                                uint8_t samples = 1 );

        // Properties
        size_t width() const;

        size_t height() const;

        uint8_t colorChannel() const;

        uint8_t samples() const;

        GLenum target() const;

        void download( GLint level, std::vector< std::uint8_t >& buffer ) const;

        // Override: Texture
        virtual void bind( uint8_t slot ) const override;

        virtual void unbind( uint8_t slot ) const override;

    private:
        Texture2D(
            size_t width, size_t height, GLint internalFormat, uint8_t levels, uint8_t samples );

        void initStorage();

        size_t m_width;
        size_t m_height;
        GLint m_internalFormat;
        uint8_t m_levels{ 1 };
        uint8_t m_samples{ 1 };
    };


    class Texture2D::Modifier
    {
    public:
        ~Modifier();

        Modifier& resize( size_t width,
                          size_t height,
                          GLint internalFormat,
                          uint8_t levels = 1,
                          uint8_t samples = 1 );

        /// Upload values into the texture
        /*
         * If dimensions do not match, the texture gets resized.
         */
        Modifier& upload( size_t width,
                          size_t height,
                          GLint internalFormat,
                          GLenum format,
                          GLenum valueType,
                          void const* data );

        /// Update a rectangular part of the texture
        Modifier& upload( size_t x,
                          size_t y,
                          size_t width,
                          size_t height,
                          GLenum format,
                          GLenum valueType,
                          void const* data );

        /// Update the texture's data
        Modifier& upload( GLenum format, GLenum valueType, void const* data );

        Modifier& download( size_t level, std::vector< uint8_t >& buffer );

        Modifier& download( size_t level, std::vector< float >& buffer );

        Modifier& minMagFilter( GLint minFilter, GLint magFilter );

        Modifier& depthMode( GLint mode );

        Modifier& compareMode( GLint mode );

        Modifier& compareFunc( GLint func );

        Modifier& wrap( GLenum xWrap, GLenum yWrap );

        Modifier& anisotropicFiltering( bool enabled );

        Modifier& multisampling( uint8_t numSamples );

        Modifier& generateMipMap();

        std::shared_ptr< Texture2D > finish();

    private:
        Modifier( std::shared_ptr< Texture2D > texture );

        std::shared_ptr< Texture2D > m_texture;

        friend Modifier modify( std::shared_ptr< Texture2D > texture );
    };


    class Texture2D::ConstModifier
    {
    public:
        ~ConstModifier();

        ConstModifier& download( size_t level, std::vector< uint8_t >& buffer );

        ConstModifier& anisotropicFiltering( bool enabled );

        void finish();

    private:
        ConstModifier( ttb::Texture2D const& texture );

        Texture2D const* m_texture;

        friend ConstModifier modify( Texture2D const& texture );
    };


    Texture2D::Modifier modify( std::shared_ptr< Texture2D > texture );

    Texture2D::ConstModifier modify( Texture2D const& texture );
}
