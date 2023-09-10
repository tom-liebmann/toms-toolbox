#pragma once

#include <ttb/core/fonts/Glyph.hpp>

#include <cstdint>
#include <map>


namespace ttb::resources
{
    class FontLoader;
}


namespace ttb
{
    /**
     * Fonts can be generated with https://github.com/Chlumsky/msdf-atlas-gen
     */
    class Font
    {
    public:
        float getEmSize() const;

        float getLineHeight() const;

        std::uint32_t getTexWidth() const;

        std::uint32_t getTexHeight() const;

        Glyph const& getGlyph( char codePoint ) const;

#ifndef TEST
    private:
#endif
        Font();

        float m_emSize;
        float m_lineHeight;
        std::uint32_t m_texWidth;
        std::uint32_t m_texHeight;
        std::map< char, Glyph > m_glyphs;

        friend class ttb::resources::FontLoader;
    };
}


namespace ttb
{
    inline float Font::getEmSize() const
    {
        return m_emSize;
    }

    inline float Font::getLineHeight() const
    {
        return m_lineHeight;
    }

    inline std::uint32_t Font::getTexWidth() const
    {
        return m_texWidth;
    }

    inline std::uint32_t Font::getTexHeight() const
    {
        return m_texHeight;
    }
}
