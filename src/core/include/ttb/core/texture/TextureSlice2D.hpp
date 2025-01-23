#pragma once

#include <ttb/math.hpp>
#include <ttb/math/Range.hpp>

#include <memory>


namespace ttb
{
    class Texture2D;
}


namespace ttb
{
    class TextureSlice2D
    {
    public:
        TextureSlice2D( std::shared_ptr< Texture2D const > texture );

        TextureSlice2D( std::shared_ptr< Texture2D const > texture, Range2F const& range );

        std::shared_ptr< Texture2D const > const& getTexture() const;

        Range2F const& getRange() const;

    private:
        std::shared_ptr< Texture2D const > m_texture;
        Range2F m_range;
    };
}


namespace ttb
{
    inline TextureSlice2D::TextureSlice2D( std::shared_ptr< Texture2D const > texture )
        : TextureSlice2D{ std::move( texture ), { { 0.0f, 0.0f }, { 1.0f, 1.0f } } }
    {
    }

    inline TextureSlice2D::TextureSlice2D( std::shared_ptr< Texture2D const > texture,
                                           Range2F const& range )
        : m_texture{ std::move( texture ) }, m_range{ range }
    {
    }

    inline auto TextureSlice2D::getRange() const -> Range2F const&
    {
        return m_range;
    }

    inline std::shared_ptr< Texture2D const > const& TextureSlice2D::getTexture() const
    {
        return m_texture;
    }
}
