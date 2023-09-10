#pragma once

#include <ttb/math/Range.hpp>


namespace ttb
{
    class Glyph
    {
    public:
        char getId() const;

        ttb::Range< float, 2 > const& getRange() const;

        ttb::Range< float, 2 > const& getTexRange() const;

        float getAdvance() const;

#ifndef TEST
    private:
#endif
        Glyph();

        char m_id;
        ttb::Range< float, 2 > m_range;
        ttb::Range< float, 2 > m_texRange;
        float m_advance;

        friend class GlyphLoader;
    };
}


namespace ttb
{
    inline Glyph::Glyph() = default;

    inline char Glyph::getId() const
    {
        return m_id;
    }

    inline ttb::Range< float, 2 > const& Glyph::getRange() const
    {
        return m_range;
    }

    inline ttb::Range< float, 2 > const& Glyph::getTexRange() const
    {
        return m_texRange;
    }

    inline float Glyph::getAdvance() const
    {
        return m_advance;
    }
}
