#pragma once

#include <ttb/core/Bindable.hpp>
#include <ttb/core/texture/Texture2D.hpp>

#include <memory>


namespace ttb
{
    class ImageBinder
    {
    public:
        virtual ~ImageBinder();

        virtual std::shared_ptr< DynamicBindable > getBindable( std::uint8_t slot ) const = 0;
    };


    class TextureBinder : public ImageBinder
    {
    public:
        TextureBinder( std::shared_ptr< Texture2D > texture );

        virtual std::shared_ptr< DynamicBindable > getBindable( std::uint8_t slot ) const override;

    private:
        std::shared_ptr< Texture2D > m_texture;
    };


    class TextureSliceBinder : public ImageBinder
    {
    public:
        TextureSliceBinder( std::shared_ptr< Texture2D > texture,
                            std::string transformName,
                            ttb::Range< float, 2 > const& range );

        virtual std::shared_ptr< DynamicBindable > getBindable( std::uint8_t slot ) const override;

    private:
        std::shared_ptr< Texture2D > m_texture;
        std::string m_transformName;
        ttb::Range< float, 2 > m_range;
    };
}
